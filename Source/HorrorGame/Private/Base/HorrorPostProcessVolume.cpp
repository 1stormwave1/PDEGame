


#include "Base/HorrorPostProcessVolume.h"

#include "Base/HorrorGameState.h"

void AHorrorPostProcessVolume::BeginPlay()
{
	Super::BeginPlay();

	if(AHorrorGameState* GameState = GetWorld()->GetGameState<AHorrorGameState>())
	{
		GameState->HorrorPPV = this;
	}
}

void AHorrorPostProcessVolume::SetPixelPostProcessValue(bool bDoActivate, float Value)
{
	if(!Settings.WeightedBlendables.Array.IsEmpty())
	{
		if(PixelizationMD == nullptr)
		{
			FWeightedBlendable& WeightedBlendable = Settings.WeightedBlendables.Array[0];
			if(UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(WeightedBlendable.Object))
			{
				PixelizationMD = UMaterialInstanceDynamic::Create(MaterialInstance, this);
			}
		}
		PixelizationMD->SetScalarParameterValue(TEXT("ShouldApply"), bDoActivate);
		PixelizationMD->SetScalarParameterValue(TEXT("PixelResolution"), Value);
		AddOrUpdateBlendable(PixelizationMD);
	}
}

void AHorrorPostProcessVolume::SetPixelPostProcessDeltaValue(float DeltaValue)
{
	CurrentPixelizationValue = FMath::Clamp(CurrentPixelizationValue + DeltaValue, MinPixelizationValue, MaxPixelizationValue);
	
	if(CurrentPixelizationValue + DeltaValue >= MaxPixelizationValue)
	{
		PixelizationMD->SetScalarParameterValue(TEXT("ShouldApply"), false);
		return;
	}
	
	if(!Settings.WeightedBlendables.Array.IsEmpty())
	{
		if(PixelizationMD == nullptr)
		{
			FWeightedBlendable& WeightedBlendable = Settings.WeightedBlendables.Array[0];
			if(UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(WeightedBlendable.Object))
			{
				PixelizationMD = UMaterialInstanceDynamic::Create(MaterialInstance, this);
			}
		}
		PixelizationMD->SetScalarParameterValue(TEXT("ShouldApply"), true);
		PixelizationMD->SetScalarParameterValue(TEXT("PixelResolution"), CurrentPixelizationValue);
		AddOrUpdateBlendable(PixelizationMD);
	}
}

void AHorrorPostProcessVolume::SetHallucinationPostProcessValue(float Value)
{
	if(!Settings.WeightedBlendables.Array.IsEmpty())
	{
		if(HallucinationMD == nullptr)
		{
			FWeightedBlendable& WeightedBlendable = Settings.WeightedBlendables.Array[1];
			if(UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(WeightedBlendable.Object))
			{
				HallucinationMD = UMaterialInstanceDynamic::Create(MaterialInstance, this);
			}
		}
		HallucinationMD->SetScalarParameterValue(TEXT("ShiftCoefficient"), Value);
		AddOrUpdateBlendable(HallucinationMD);
	}
}

void AHorrorPostProcessVolume::SetNightVisionPostProcessValue(float Value)
{
	Settings.AutoExposureBias = 6.5f + 3.5f * Value;
	Settings.ColorSaturation = FVector4(1.f + 1.f * Value, 1.f + 1.f * Value, 1.f + 1.f * Value, 1.f);
}
