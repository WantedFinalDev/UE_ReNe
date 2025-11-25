#include "Global/Rene_Booth_GameMode.h"

void ARene_Booth_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	FInputModeGameOnly im;
	NewPlayer->bShowMouseCursor = false;
	NewPlayer->SetInputMode(im);
}
