#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OnlineSubsystem.h"
#include "Rene_VoiceChatManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_RENE_API URene_VoiceChatManager : public UActorComponent
{
	GENERATED_BODY()

	/* Method */
public:
	URene_VoiceChatManager();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartVoice();

	UFUNCTION(BlueprintCallable)
	void StopVoice();

	UFUNCTION(NetMulticast, Reliable)
	void EstablishPrivateVoiceChannel(APlayerState* PlayerA, APlayerState* PlayerB);

	UFUNCTION(NetMulticast, Reliable)
	void EndPrivateVoiceChannel();
	
	
private:
	
	
	
	
	
	
	/* Field */
public:
	
	
	
	
	
private:
	IOnlineSubsystem* Subsystem;
	IOnlineVoicePtr VoiceInterface;

	int32 LocalUserNum = 0;
	
	
	
	

};
