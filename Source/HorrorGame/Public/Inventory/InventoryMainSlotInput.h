

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryMainSlotInput.generated.h"


UCLASS()
class HORRORGAME_API UInventoryMainSlotInput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	class UInventoryComponent* InventoryComponent = nullptr;
	
	int32 SlotIndexToActivate = 0;

	UFUNCTION()
	void ActivateSlot();
};
