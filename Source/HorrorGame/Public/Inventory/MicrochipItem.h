

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
#include "Building/Room.h"
#include "MicrochipItem.generated.h"

UENUM(BlueprintType)
enum class EMicrochipType : uint8
{
	None,
	Alpha,
	Beta,
	Gamma
};

UCLASS()
class HORRORGAME_API UMicrochipItem : public UItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsRoomEffectActivated = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMicrochipType MicrochipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ERoomType> CompatibleRoomTypes;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateCompatibility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisactivateCompatibility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRoomChanged(ARoom* Room);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateRoomEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisactivateRoomEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool ShouldActivateRoomEffect(ARoom* Room);
};
