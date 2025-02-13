

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMicrochipType MicrochipType;
};
