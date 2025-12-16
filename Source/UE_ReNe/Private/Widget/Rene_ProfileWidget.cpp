#include "Widget/Rene_ProfileWidget.h"

#include "Components/Button.h"
#include "Player/Rene_PlayerController.h"
#include "Network/Rene_FileUploader.h"
#include "Global/Rene_GameInstance.h" // GameInstance 헤더 추가

void URene_ProfileWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_Upload->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickUpload);
	btn_DashBoard->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickDashBoard);
	btn_Return->OnClicked.AddDynamic(this, &URene_ProfileWidget::OnClickReturn);
	
}

void URene_ProfileWidget::OnClickUpload()
{
	ARene_PlayerController* PC = Cast<ARene_PlayerController>(GetOwningPlayer());
	if (PC)
	{
		FString FilePath;
		if (PC->ShowFileDialog(TEXT("Select File to Upload"), TEXT(""), TEXT(""), FilePath))
		{
			if (PC->FileUploader)
			{
				// GameInstance에서 사용자 데이터 가져오기
				URene_GameInstance* GameInstance = GetGameInstance<URene_GameInstance>();
				if (GameInstance)
				{
					FReneUserData UserData = GameInstance->GetCachedUserData();
					//FString UserId = UserData.ID; // UserData의 ID를 공통 UserId로 사용
					FString UserId = "1"; // UserData의 ID를 공통 UserId로 사용


					// UserData.Role 값에 따라 업로드 타입을 결정합니다.
					if (UserData.Role == TEXT("Company"))
					{
						PC->FileUploader->StartFileUpload(FilePath, EUploadUserType::Company, UserId);
					}
					if (UserData.Role == TEXT("Seeker"))
					{
						PC->FileUploader->StartFileUpload(FilePath, EUploadUserType::JobSeeker, UserId);
					}
				}
			}
		}
	}
}

void URene_ProfileWidget::OnClickDashBoard()
{
	//	OnClickDelDash.Broadcast();
	OnClickDashDynamic.Broadcast();
}

void URene_ProfileWidget::OnClickReturn()
{
	OnClickReturnDynamic.Broadcast();
}
