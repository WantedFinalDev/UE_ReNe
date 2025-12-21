#pragma once

#include "CoreMinimal.h"
#include "ReneUserData.generated.h"

USTRUCT(BlueprintType)
struct FReneUserData
{
	GENERATED_BODY()

	/*
	 *	11.28.금 20:29
	 *	Login Data 가상의 구조
	 */
	UPROPERTY(BlueprintReadWrite)
	FString ID = TEXT("EmptyID");
	
	UPROPERTY(BlueprintReadWrite)
	FString Name = TEXT("EmptyName");
	
	UPROPERTY(BlueprintReadWrite)
	int32 Level = 1;
	
	UPROPERTY(BlueprintReadWrite)
	FString Role = TEXT("CompanyOrSeeker");
};
