

#include "Inventory/InventoryMainSlotInput.h"

#include "Base/Components/InventoryComponent.h"

void UInventoryMainSlotInput::ActivateSlot()
{
	if(IsValid(InventoryComponent))
	{
		InventoryComponent->SetActiveSlotIndex(SlotIndexToActivate);
	}
}
