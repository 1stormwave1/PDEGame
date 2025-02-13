

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemDDOPayload.generated.h"

class UItem;

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	None,
	Inventory,
	Body,
	Microchip
};

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UInventoryItemDDOPayload : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlotType SlotType;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItem> Item;
};
