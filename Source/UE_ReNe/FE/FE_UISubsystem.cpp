#include "FE_UISubsystem.h"

#include "Engine/AssetManager.h"
#include "FE/FE_DubugHelper.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "FE/Widget_ActivatableBase.h"
#include "FE/Widget_PrimaryLayout.h"

UFE_UISubsystem* UFE_UISubsystem::GetUISubsystem(const UObject* WorldContextObject)
{
	// GEngine은 언리얼의 전역 엔진 포인터다. IsValid()로 null 및 가비지 컬렉션 대기 상태를 동시에 검사해 엔진이 완전히 살아있을 때만 진입한다.
	if (IsValid(GEngine))
	{
		// WorldContextObject를 기반으로 현재 월드를 가져온다. Assert 모드이므로 월드를 찾지 못하면 크래시가 발생해 디버깅 시 즉시 문제를 알 수 있다.
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		// 현재 월드의 GameInstance에서 UFE_UISubsystem 서브시스템 인스턴스를 꺼내 반환한다. 서브시스템은 GameInstance 생애주기 동안 단 하나만 존재한다.
		return UGameInstance::GetSubsystem<UFE_UISubsystem>(World->GetGameInstance());
	}
	// GEngine이 유효하지 않은 경우(예: 에디터 초기화 이전)에는 안전하게 nullptr을 반환한다.
	return nullptr;
}

bool UFE_UISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// 이 서브시스템을 생성해야 하는지 엔진이 판단할 때 호출한다. false를 반환하면 해당 환경에서 인스턴스가 아예 만들어지지 않는다.
	// Outer는 이 서브시스템을 소유할 GameInstance다. CastChecked는 캐스트 실패 시 크래시를 내므로 타입이 확실할 때만 사용한다.
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// 이 클래스를 상속한 자식 클래스 목록을 담을 배열이다.
		TArray<UClass*> FoundClasses;
		// UFE_UISubsystem을 상속한 파생 클래스가 있는지 검색한다.
		GetDerivedClasses(GetClass(), FoundClasses);

		// 파생 클래스가 없을 때만 true를 반환한다. 자식 클래스가 있으면 부모 대신 자식이 생성되어야 하므로 이 클래스는 생성을 포기한다.
		return FoundClasses.IsEmpty();
	}

	// 전용 서버(Dedicated Server)에서는 UI가 필요 없으므로 생성하지 않는다.
	return false;
}

void UFE_UISubsystem::RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);
	CreatedPrimaryLayout = InCreatedWidget;
	FEDebug::Print(TEXT("Regist Complete : Primary Layout Widget"));
}

void UFE_UISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState,UWidget_ActivatableBase*)> AsyncPushStateCallback)
{
	// TSoftClassPtr은 에셋을 메모리에 올리지 않은 상태의 참조다. IsNull()은 경로 자체가 없는 경우를 잡아낸다.
	check(!InSoftWidgetClass.IsNull());

	// 에셋을 실제로 메모리에 올리는 비동기 로드 요청. 로드가 완료되면 람다가 게임 스레드에서 호출된다.
	// 메인 스레드를 블로킹하지 않으므로 로딩 중에도 게임이 멈추지 않는다.
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad
	(
		// TSoftClassPtr을 FSoftObjectPath로 변환해 스트리밍 관리자에게 전달한다.
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda
		(
			// 람다 캡처 목록: 로드 완료 시점에도 필요한 값들을 값 복사(by value)로 캡처한다.
			// this는 포인터 캡처이므로, 로드 완료 전 서브시스템이 파괴되는 상황을 주의해야 한다.
			[InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallback]()
			{
				// 비동기 로드가 끝난 시점에 실제 UClass 포인터를 꺼낸다. 로드 실패 시 nullptr이 된다.
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();

				// 클래스 로드 실패 또는 PrimaryLayout이 아직 등록되지 않은 경우를 모두 방어한다.
				check(LoadedWidgetClass && CreatedPrimaryLayout);

				// GameplayTag를 키로 삼아 PrimaryLayout에 등록된 위젯 스택 컨테이너를 찾는다.
				// 스택이 없으면 nullptr이 반환되므로 호출 전 태그가 올바르게 등록되어 있어야 한다.
				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

				// AddWidget은 위젯을 생성하고 스택에 추가한다. 두 번째 인자 콜백은 위젯 생성 직후, 스택 추가 및 Activate 이전에 호출된다.
				// FoundWidgetStack이 nullptr이면 여기서 크래시가 발생한다. 태그가 PrimaryLayout에 올바르게 등록되어 있어야 한다.
				UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UWidget_ActivatableBase>
				(
					LoadedWidgetClass,
					[AsyncPushStateCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
					{
						// 위젯이 생성된 직후, 스택 추가 및 Activate 이전에 호출된다. 초기값 주입 등 사전 처리를 여기서 수행한다.
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				// 위젯이 스택에 추가되어 화면에 표시된 이후 호출된다. 표시 후 추가 작업이 필요한 경우 여기서 처리한다.
				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}

