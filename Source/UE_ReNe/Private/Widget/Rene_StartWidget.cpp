#include "Widget/Rene_StartWidget.h"

#include "UE_ReNe.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Global/Rene_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void URene_StartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	
	btn_Enter->OnClicked.AddDynamic(this, &URene_StartWidget::OnClickedEnter);
	btn_Exit->OnClicked.AddDynamic(this, &URene_StartWidget::OnClickedExit);
}

void URene_StartWidget::OnClickedEnter()
{
	/* TODO : Autho Login
	 * 
	 * 11.28.금 20:29 비둘기
	 * GameInstance 에 로그인 데이터 저장
	 * Booth_GameMode 시작시, PostLogin 에서 UserData 활용
	 * Booth_GM으로 가시오
	 */
	
	TObjectPtr<URene_GameInstance> gi = Cast<URene_GameInstance>(GetGameInstance());
	if (IsValidChecked(gi))
	{
		// 아직 Login Data가 없어 가상의 코드로 대체함
		gi->SetReneUserData(etxt_ID->GetText().ToString(), etxt_ID->GetText().ToString(), 1);
		UGameplayStatics::OpenLevel(GetWorld(), "LobbyMap");
	}
	else
	{
		LOGERROR()
	}
	
	/*APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->ClientTravel("/Game/Maps/LobbyMap", TRAVEL_Absolute);*/
}

void URene_StartWidget::OnClickedExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}