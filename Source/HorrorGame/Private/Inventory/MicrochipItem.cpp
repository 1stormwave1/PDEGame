


#include "Inventory/MicrochipItem.h"
#include "Building/BuildingGameState.h"

void UMicrochipItem::ActivateCompatibility_Implementation()
{
	if(ABuildingGameState* GameState = GetWorld()->GetGameState<ABuildingGameState>())
	{
		GameState->OnCurrentRoomChangedDelegate.AddDynamic(this, &UMicrochipItem::OnRoomChanged);
		OnRoomChanged(GameState->CurrentRoom);
	}
}

void UMicrochipItem::DisactivateCompatibility_Implementation()
{
	if(ABuildingGameState* GameState = GetWorld()->GetGameState<ABuildingGameState>())
	{
		GameState->OnCurrentRoomChangedDelegate.RemoveDynamic(this, &UMicrochipItem::OnRoomChanged);
		OnRoomChanged(GameState->CurrentRoom);
	}
}

void UMicrochipItem::OnRoomChanged_Implementation(APCGRoom* Room)
{
	if(ShouldActivateRoomEffect(Room))
	{
		ActivateRoomEffect();
	}
	else
	{
		DisactivateRoomEffect();
	}
}

void UMicrochipItem::ActivateRoomEffect_Implementation()
{
	if(!bIsRoomEffectActivated)
	{
		bIsRoomEffectActivated = true;
	}
}

void UMicrochipItem::DisactivateRoomEffect_Implementation()
{
	if(bIsRoomEffectActivated)
    {
		bIsRoomEffectActivated = false;
    }
}

bool UMicrochipItem::ShouldActivateRoomEffect_Implementation(APCGRoom* Room)
{
	return IsValid(Room) && CompatibleRoomTypes.Contains(Room->Type);
}
