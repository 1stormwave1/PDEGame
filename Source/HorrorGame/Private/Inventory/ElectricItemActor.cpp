


#include "Inventory/ElectricItemActor.h"

#include "Inventory/Item.h"

void AElectricItemActor::StartFeedbackLoop_Implementation()
{
}

void AElectricItemActor::EndFeedbackLoop_Implementation()
{
}

void AElectricItemActor::StartOvercharge_Implementation()
{
}

void AElectricItemActor::EndOvercharge_Implementation()
{
}

void AElectricItemActor::BurnOut_Implementation()
{
	if(IsValid(ItemOwner))
	{
		ItemOwner->BurnOutPower();
	}
}

void AElectricItemActor::FeedbackLoopTick_Implementation()
{
}

void AElectricItemActor::SetItemActorParams_Implementation(UItem* Item)
{
	Super::SetItemActorParams_Implementation(Item);

	if(IsValid(Item))
	{
		bIsBurnOut = Item->bIsBurnOut;
		Item->OnBurnOutChanged.AddDynamic(this, &AElectricItemActor::OnBurnOutChanged);
	}
}

void AElectricItemActor::OnBurnOutChanged(bool bBurnOut)
{
	if(bBurnOut)
	{
		BurnOut();
	}
}
