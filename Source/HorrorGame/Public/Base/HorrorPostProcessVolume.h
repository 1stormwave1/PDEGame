

#pragma once

#include "CoreMinimal.h"
#include "Engine/PostProcessVolume.h"
#include "HorrorPostProcessVolume.generated.h"


UCLASS()
class HORRORGAME_API AHorrorPostProcessVolume : public APostProcessVolume
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UMaterialInstanceDynamic> PixelizationMD = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float MaxPixelizationValue = 200.f;

	UPROPERTY(BlueprintReadWrite)
	float MinPixelizationValue = 15.f;

	UPROPERTY(BlueprintReadWrite)
	float CurrentPixelizationValue = 200.f;

	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UMaterialInstanceDynamic> HallucinationMD = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	void SetPixelPostProcessValue(bool bDoActivate, float Value);

	UFUNCTION(BlueprintCallable)
	void SetPixelPostProcessDeltaValue(float DeltaValue);

	UFUNCTION(BlueprintCallable)
	void SetHallucinationPostProcessValue(float Value);

	UFUNCTION(BlueprintCallable)
	void SetNightVisionPostProcessValue(float Value);
};
