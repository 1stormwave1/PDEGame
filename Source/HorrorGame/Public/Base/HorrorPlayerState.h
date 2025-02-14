

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HorrorPlayerState.generated.h"


class AHorrorPostProcessVolume;

UCLASS()
class HORRORGAME_API AHorrorPlayerState : public APlayerState
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
