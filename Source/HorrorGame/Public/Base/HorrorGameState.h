

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HorrorGameState.generated.h"


class AHorrorPostProcessVolume;

UCLASS()
class HORRORGAME_API AHorrorGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<AHorrorPostProcessVolume> HorrorPPV = nullptr;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartHallucinate();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndHallucinate();
	
};
