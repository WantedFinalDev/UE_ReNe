#include "FE_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void AFE_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	TArray<AActor*> SearchedCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), FName("Default"), SearchedCameras);
	
	if (!SearchedCameras.IsEmpty())
	{
		SetViewTarget(SearchedCameras[0]);
	}
}
