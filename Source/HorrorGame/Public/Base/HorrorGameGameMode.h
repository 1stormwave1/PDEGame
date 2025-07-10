

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HorrorGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPCGStageChangedSignature);

UCLASS(minimalapi)
class AHorrorGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHorrorGameGameMode();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPCGStageChangedSignature OnPCGStarted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPCGStageChangedSignature OnPCGFinished;
};



