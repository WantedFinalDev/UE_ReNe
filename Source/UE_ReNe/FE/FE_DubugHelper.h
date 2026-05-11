#pragma once
#include "UE_ReNe.h"

namespace FEDebug
{
	static void Print(const FString& Msg, int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, InColor, Msg);
			LOGERRORF(TEXT("%s"), *Msg);
		}
	}
}
