
#include "Base/Components/InventoryComponent.h"
#include "Inventory/Item.h"
#include "Inventory/ItemActor.h"
#include "Base/HorrorGameController.h"
#include "Inventory/ElectricItemActor.h"
#include "Inventory/FillUpItem.h"


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for(FInventorySlot& Slot : MainSlots)
	{
		if(Slot.CurrentItemClass != nullptr)
		{
			Slot.CurrentItem = NewObject<UItem>(this, Slot.CurrentItemClass);
		}
	}
	for(FInventorySlot& Slot : InventorySlots)
	{
		if(Slot.CurrentItemClass != nullptr)
		{
			Slot.CurrentItem = NewObject<UItem>(this, Slot.CurrentItemClass);
		}
	}
}

void UInventoryComponent::ClearInventorySlot(int32 Index, TArray<FInventorySlot>& InventoryArray)
{
	if(InventoryArray.IsValidIndex(Index))
	{
		InventoryArray[Index].CurrentItem = nullptr;
		InventoryArray[Index].CurrentItemClass = nullptr;
	}
}

void UInventoryComponent::ClearNonQASlot(int32 Index)
{
	ClearInventorySlot(Index, InventorySlots);
	OnInventorySlotCleared.Broadcast(Index);
	OnInventorySlotItemChanged.Broadcast(Index);
}

void UInventoryComponent::ClearQASlot(int32 Index)
{
	ClearInventorySlot(Index, MainSlots);
	OnMainSlotCleared.Broadcast(Index);
	OnMainInventorySlotItemChanged.Broadcast(Index);

	if(Index == ActiveSlotIndex)
	{
		OnActiveItemChanged.Broadcast(Index);
	}
}

void UInventoryComponent::ClearAdditionalSlot(int32 Index)
{
	ClearInventorySlot(Index, AdditionalInventorySlots);
	OnAdditionalSlotCleared.Broadcast(Index);
	OnAdditionalSlotItemChanged.Broadcast(Index);
}

void UInventoryComponent::ClearBodySlot(int32 Index)
{
	if(BodySlots.IsValidIndex(Index))
	{
		BodySlots[Index].CurrentItem = nullptr;
		BodySlots[Index].CurrentItemClass = nullptr;

		OnBodySlotCleared.Broadcast(Index);
		OnBodySlotItemChanged.Broadcast(Index);
	}
}

void UInventoryComponent::ClearMicrochipSlot(int32 Index)
{
	if(MicrochipSlots.IsValidIndex(Index))
	{
		MicrochipSlots[Index].CurrentItem = nullptr;
		MicrochipSlots[Index].CurrentItemClass = nullptr;

		OnMicrochipSlotCleared.Broadcast(Index);
		OnMicrochipSlotItemChanged.Broadcast(Index);
	}
}

void UInventoryComponent::PutItemInInventory(UItem* Item, int32 Index, TArray<FInventorySlot>& InventoryArray)
{
	if(InventoryArray.IsValidIndex(Index))
	{
		InventoryArray[Index].CurrentItem = Item;
		InventoryArray[Index].CurrentItemClass = Item->GetClass();
	}
}

void UInventoryComponent::PutItemInNonQASlots(UItem* Item, int32 Index)
{
	PutItemInInventory(Item, Index, InventorySlots);
	OnItemPutInInventorySlot.Broadcast(Index, Item);
	OnInventorySlotItemChanged.Broadcast(Index);
}

void UInventoryComponent::PutItemInQASlots(UItem* Item, int32 Index)
{
	PutItemInInventory(Item, Index, MainSlots);
	OnItemPutInMainSlot.Broadcast(Index, Item);
	OnMainInventorySlotItemChanged.Broadcast(Index);

	if(Index == ActiveSlotIndex)
	{
		OnActiveItemChanged.Broadcast(Index);
	}
}

void UInventoryComponent::PutItemInAdditionalSlots(UItem* Item, int32 Index)
{
	PutItemInInventory(Item, Index, AdditionalInventorySlots);
	OnItemPutInAdditionalSlot.Broadcast(Index, Item);
	OnAdditionalSlotItemChanged.Broadcast(Index);
}

void UInventoryComponent::PutItemInBodySlots(UItem* Item, int32 Index)
{
	if(BodySlots.IsValidIndex(Index))
	{
		BodySlots[Index].CurrentItem = Item;
		BodySlots[Index].CurrentItemClass = Item->GetClass();

		OnItemPutInBodySlot.Broadcast(Index, Item);
		OnBodySlotItemChanged.Broadcast(Index);
	}
}

bool UInventoryComponent::PutItemInBodySlotByType(UItem* Item, EBodyType Type)
{
	if(!IsValid(Item))
	{
		return false;
	}
	
	if(const UBodyItem* BodyItem = Cast<UBodyItem>(Item))
	{
		if(BodyItem->BodyType == Type)
		{
			for(int i = 0; i < BodySlots.Num(); ++i)
			{
				if(BodySlots[i].BodyType == Type && BodySlots[i].CurrentItem == nullptr)
				{
					PutItemInBodySlots(Item, i);
					return true;
				}
			}
		}
	}
	return false;
}

bool UInventoryComponent::PutItemInBodySlotByTypeWithActor(AItemActor* Actor, UItem* Item, EBodyType Type)
{
	if(!IsValid(Item))
	{
		return false;
	}
	
	if(const UBodyItem* BodyItem = Cast<UBodyItem>(Item))
	{
		if(BodyItem->BodyType == Type)
		{
			for(int i = 0; i < BodySlots.Num(); ++i)
			{
				if(BodySlots[i].BodyType == Type && BodySlots[i].CurrentItem == nullptr)
				{
					BodySlots[i].ItemActor = Actor;
					PutItemInBodySlots(Item, i);
					return true;
				}
			}
		}
	}
	return false;
}

void UInventoryComponent::PutItemInMicrochipSlots(UItem* Item, int32 Index)
{
	if(MicrochipSlots.IsValidIndex(Index))
	{
		MicrochipSlots[Index].CurrentItem = Item;
		MicrochipSlots[Index].CurrentItemClass = Item->GetClass();

		OnItemPutInMicrochipSlot.Broadcast(Index, Item);
		OnMicrochipSlotItemChanged.Broadcast(Index);
	}
}

void UInventoryComponent::SetActiveSlotIndex(const int32 NewActiveSlotIndex)
{
	if(NewActiveSlotIndex > MainSlots.Num() - 1)
	{
		ActiveSlotIndex = 0;
	}
	else if(NewActiveSlotIndex < 0)
	{
		ActiveSlotIndex = MainSlots.Num() - 1;
	}
	else
	{
		ActiveSlotIndex = NewActiveSlotIndex;
	}
	
	OnActiveSlotIndexChanged.Broadcast(ActiveSlotIndex);
}

void UInventoryComponent::SpawnActiveSlotItem(USceneComponent* AttachComponent)
{
	UItem* Item = MainSlots[ActiveSlotIndex].CurrentItem;
	if(IsValid(AttachComponent) && IsValid(Item))
	{
		if(const UBodyItem* BodyItem = Cast<UBodyItem>(Item))
		{
			CurrentItemActor = GetItemActorByBodyType(BodyItem->BodyType);
		}
		else if(const UMicrochipItem* MicrochipItem = Cast<UMicrochipItem>(Item))
		{
			CurrentItemActor = GetItemActorByMicrochipType(MicrochipItem->MicrochipType);
		}
		if(!IsValid(CurrentItemActor))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = OwnerController->GetPawn();
			CurrentItemActor = GetWorld()->SpawnActor<AItemActor>(Item->ActorClass, SpawnParams);
			CurrentItemActor->SetItemActorParams(Item);
		}
		CurrentItemActor->AttachToComponent(AttachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

bool UInventoryComponent::PickUpItem(UItem* Item)
{
	if(!IsValid(Item))
	{
		return false;
	}
	
	UItem* NewItem = NewObject<UItem>(this, Item->GetClass());
	if(Item->bIsLimitedPower)
	{
		NewItem->MaxPower = Item->MaxPower;
		NewItem->CurrentPower = Item->CurrentPower;
		NewItem->bIsBurnOut = Item->bIsBurnOut;
		NewItem->bIsBurnOutPower = Item->bIsBurnOutPower;
	}
	
	for(int32 i = 0; i < MainSlots.Num(); ++i)
	{
		if(!IsValid(MainSlots[i].CurrentItem))
		{
			PutItemInQASlots(NewItem, i);
			OnItemPickedUp.Broadcast(NewItem->GetClass());
			return true;
		}
	}
	for(int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if(!IsValid(InventorySlots[i].CurrentItem))
		{
			PutItemInNonQASlots(NewItem, i);
			OnItemPickedUp.Broadcast(NewItem->GetClass());
			return true;
		}
	}
	if(bIsAdditionalSlotsActive)
	{
		for(int32 i = 0; i < AdditionalInventorySlots.Num(); ++i)
		{
			if(!IsValid(AdditionalInventorySlots[i].CurrentItem))
			{
				PutItemInAdditionalSlots(NewItem, i);
				OnItemPickedUp.Broadcast(NewItem->GetClass());
				return true;
			}
		}
	}
	if(UBodyItem* BodyItem = Cast<UBodyItem>(Item))
	{
		for(int32 i = 0; i < BodySlots.Num(); ++i)
		{
			if(BodySlots[i].BodyType == BodyItem->BodyType)
			{
				if(!IsValid(BodySlots[i].CurrentItem))
				{
					PutItemInBodySlots(NewItem, i);
					OnItemPickedUp.Broadcast(NewItem->GetClass());
					return true;
				}
			}
		}
	}
	if(UMicrochipItem* MicrochipItem = Cast<UMicrochipItem>(Item))
	{
		for(int32 i = 0; i < MicrochipSlots.Num(); ++i)
		{
			if(MicrochipSlots[i].MicrochipType == MicrochipItem->MicrochipType)
			{
				if(!IsValid(MicrochipSlots[i].CurrentItem))
				{
					PutItemInMicrochipSlots(NewItem, i);
					OnItemPickedUp.Broadcast(NewItem->GetClass());
					return true;
				}
			}
		}
		for(int32 i = 0; i < MicrochipSlots.Num(); ++i)
		{
			if(!IsValid(MicrochipSlots[i].CurrentItem))
			{
				PutItemInMicrochipSlots(NewItem, i);
				OnItemPickedUp.Broadcast(NewItem->GetClass());
				return true;
			}
		}
	}
	
	OnItemPickedUp.Broadcast(nullptr);
	return false;
}

void UInventoryComponent::DestroyActiveSlotItem()
{
	if(IsValid(CurrentItemActor))
	{
		CurrentItemActor->FinishUseItem();
		CurrentItemActor->ItemOwner->StopUsePower();
		CurrentItemActor->Destroy();
	}
}

void UInventoryComponent::GetBodySlotByType(EBodyType Type, FInventoryBodySlot& Slot)
{
	for(const FInventoryBodySlot& InSlot : BodySlots)
	{
		if(InSlot.BodyType == Type)
		{
			Slot = InSlot;
			return;
		}
	}
}

void UInventoryComponent::SetOwnerController(AHorrorGameController* NewOwnerController)
{
	OwnerController = NewOwnerController;
}

void UInventoryComponent::ActivateBodyActor(AItemActor* Actor, EBodyType BodyType)
{
	for(FInventoryBodySlot& Slot : BodySlots)
	{
		if(Slot.BodyType == BodyType)
		{
			Slot.ItemActor = Actor;
			if(!IsValid(Slot.ItemActor))
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = OwnerController->GetPawn();

				Slot.ItemActor = GetWorld()->SpawnActor<AItemActor>(Slot.CurrentItem->ActorClass, SpawnParams);
				Slot.ItemActor->SetItemActorParams(Slot.CurrentItem);

				Actor = Slot.ItemActor;
			}
			break;
		}
	}
	Actor->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	Actor->AttachToActor(
		OwnerController->GetPawn(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
}

AItemActor* UInventoryComponent::GetItemActorByBodyType(EBodyType BodyType)
{
	for(FInventoryBodySlot& Slot : BodySlots)
	{
		if(Slot.BodyType == BodyType)
		{
			return Slot.ItemActor;
		}
	}
	return nullptr;
}

void UInventoryComponent::SetItemActorForBodyType(AItemActor* Actor, EBodyType BodyType)
{
	for(FInventoryBodySlot& Slot : BodySlots)
	{
		if(Slot.BodyType == BodyType)
		{
			Slot.ItemActor = Actor;
			return;
		}
	}
}

AItemActor* UInventoryComponent::GetItemActorByMicrochipType(EMicrochipType MicrochipType)
{
	for(FInventoryMicrochipSlot& Slot : MicrochipSlots)
	{
		if(Slot.MicrochipType == MicrochipType)
		{
			return Slot.ItemActor;
		}
	}
	return nullptr;
}

void UInventoryComponent::GetAllFillUpItems(UItem* PowerItem, TArray<UFillUpItem*>& FillUpItems)
{
	if(!PowerItem)
	{
		return;
	}
	for(FInventorySlot& MainSlot : MainSlots)
	{
		if(!MainSlot.CurrentItem)
		{
			continue;
		}
		if(PowerItem->FillUpItems.Contains(MainSlot.CurrentItem->GetClass()))
		{
			if(!FillUpItems.ContainsByPredicate([MainSlot](const UFillUpItem* InItem)
			{
				return MainSlot.CurrentItem->GetClass() == InItem->GetClass();
			}))
			{
				FillUpItems.Add(Cast<UFillUpItem>(MainSlot.CurrentItem));
			}
		}
	}
	for(FInventorySlot& Slot : InventorySlots)
	{
		if(!Slot.CurrentItem)
		{
			continue;
		}
		if(PowerItem->FillUpItems.Contains(Slot.CurrentItem->GetClass()))
		{
			if(!FillUpItems.ContainsByPredicate([Slot](const UFillUpItem* InItem)
			{
				return Slot.CurrentItem->GetClass() == InItem->GetClass();
			}))
			{
				FillUpItems.Add(Cast<UFillUpItem>(Slot.CurrentItem));
			}
		}
	}
	for(FInventorySlot& Slot : AdditionalInventorySlots)
	{
		if(!Slot.CurrentItem)
		{
			continue;
		}
		if(PowerItem->FillUpItems.Contains(Slot.CurrentItem->GetClass()))
		{
			if(!FillUpItems.ContainsByPredicate([Slot](const UFillUpItem* InItem)
			{
				return Slot.CurrentItem->GetClass() == InItem->GetClass();
			}))
			{
				FillUpItems.Add(Cast<UFillUpItem>(Slot.CurrentItem));
			}
		}
	}
}

void UInventoryComponent::GetAllLimitedPowerItems(UFillUpItem* FillUpItem, TArray<UItem*>& PowerItems)
{
	if(!FillUpItem)
	{
		return;
	}
	for(FInventorySlot& MainSlot : MainSlots)
	{
		if(!MainSlot.CurrentItem)
		{
			continue;
		}
		if(MainSlot.CurrentItem->bIsLimitedPower)
		{
			if(MainSlot.CurrentItem->FillUpItems.Contains(FillUpItem->GetClass()))
			{
				if(MainSlot.CurrentItem->CurrentPower < MainSlot.CurrentItem->MaxPower)
				{
					PowerItems.Add(MainSlot.CurrentItem);
				}
			}
		}
	}
	for(FInventorySlot& Slot : InventorySlots)
	{
		if(!Slot.CurrentItem)
		{
			continue;
		}
		if(Slot.CurrentItem->bIsLimitedPower)
		{
			if(Slot.CurrentItem->FillUpItems.Contains(FillUpItem->GetClass()))
			{
				if(Slot.CurrentItem->CurrentPower < Slot.CurrentItem->MaxPower)
				{
					PowerItems.Add(Slot.CurrentItem);
				}
			}
		}
	}
	for(FInventorySlot& Slot : AdditionalInventorySlots)
	{
		if(!Slot.CurrentItem)
		{
			continue;
		}
		if(Slot.CurrentItem->bIsLimitedPower)
		{
			if(Slot.CurrentItem->FillUpItems.Contains(FillUpItem->GetClass()))
			{
				if(Slot.CurrentItem->CurrentPower < Slot.CurrentItem->MaxPower)
				{
					PowerItems.Add(Slot.CurrentItem);
				}
			}
		}
	}
}

void UInventoryComponent::RemoveItemFromInventory(UItem* Item)
{
	if(!Item)
	{
		return;
	}
	for(int32 i = 0; i < MainSlots.Num(); ++i)
	{
		if(MainSlots[i].CurrentItem == Item)
		{
			ClearQASlot(i);
			return;
		}
	}
	for(int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if(InventorySlots[i].CurrentItem == Item)
		{
			ClearNonQASlot(i);
			return;
		}
	}
	for(int32 i = 0; i < AdditionalInventorySlots.Num(); ++i)
	{
		if(AdditionalInventorySlots[i].CurrentItem == Item)
		{
			ClearAdditionalSlot(i);
			return;
		}
	}
	
}

void UInventoryComponent::GetAllActiveElectricItemActors(TArray<AElectricItemActor*>& ElectricItemActors)
{
	if(AElectricItemActor* ElectricItemActor = Cast<AElectricItemActor>(CurrentItemActor))
	{
		ElectricItemActors.Add(ElectricItemActor);
	}
	for(FInventoryBodySlot& Slot : BodySlots)
	{
		if(AElectricItemActor* ElectricItemActor = Cast<AElectricItemActor>(Slot.ItemActor))
		{
			ElectricItemActors.Add(ElectricItemActor);
		}
	}
	for(FInventoryMicrochipSlot& Slot : MicrochipSlots)
	{
		if(AElectricItemActor* ElectricItemActor = Cast<AElectricItemActor>(Slot.ItemActor))
		{
			ElectricItemActors.Add(ElectricItemActor);
		}
	}
}

bool UInventoryComponent::DoesContainItem(UItem* Item)
{
	for(FInventorySlot& Slot : MainSlots)
	{
		if(Slot.CurrentItem != nullptr && Slot.CurrentItem == Item)
		{
			return true;
		}
	}
	for(FInventorySlot& Slot : InventorySlots)
	{
		if(Slot.CurrentItem != nullptr && Slot.CurrentItem == Item)
		{
			return true;
		}
	}
	for(FInventorySlot& Slot : AdditionalInventorySlots)
	{
		if(Slot.CurrentItem != nullptr && Slot.CurrentItem == Item)
		{
			return true;
		}
	}
	for(FInventoryBodySlot& Slot : BodySlots)
	{
		if(Slot.CurrentItem != nullptr && Slot.CurrentItem == Item)
		{
			return true;
		}
	}
	for(FInventoryMicrochipSlot& Slot : MicrochipSlots)
	{
		if(Slot.CurrentItem != nullptr && Slot.CurrentItem == Item)
		{
			return true;
		}
	}
	return false;
}

void UInventoryComponent::DropItemFromMainSlots(int32 Index, FVector Location)
{
	if(MainSlots.IsValidIndex(Index))
	{
		FInventorySlot& Slot = MainSlots[Index];
	
		if(IsValid(CurrentItemActor) && CurrentItemActor->ItemOwner == Slot.CurrentItem)
		{
			//CurrentItemActor->Destroy();
			OnActiveItemChanged.Broadcast(Index);
		}
		DropItem(Slot, Location);
		OnMainSlotCleared.Broadcast(Index);
	}
}

void UInventoryComponent::DropItemFromInventorySlots(int32 Index, FVector Location)
{
	if(InventorySlots.IsValidIndex(Index))
	{
		FInventorySlot& Slot = InventorySlots[Index];
		DropItem(Slot, Location);
		OnInventorySlotCleared.Broadcast(Index);
	}
}

void UInventoryComponent::DropItemFromAdditionalSlots(int32 Index, FVector Location)
{
	if(AdditionalInventorySlots.IsValidIndex(Index))
	{
		FInventorySlot& Slot = AdditionalInventorySlots[Index];
		DropItem(Slot, Location);
		OnAdditionalSlotCleared.Broadcast(Index);
	}
}

void UInventoryComponent::DropItemFromBodySlots(int32 Index, FVector Location)
{
	if(BodySlots.IsValidIndex(Index))
	{
		FInventoryBodySlot& Slot = BodySlots[Index];
	
		if(IsValid(Slot.ItemActor))
		{
			Slot.ItemActor->Destroy();
		}
		DropItem(Slot, Location);
		OnBodySlotCleared.Broadcast(Index);
	}
}

void UInventoryComponent::DropItemFromMicrochipSlots(int32 Index, FVector Location)
{
	if(MicrochipSlots.IsValidIndex(Index))
	{
		FInventoryMicrochipSlot& Slot = MicrochipSlots[Index];
	
		if(IsValid(Slot.ItemActor))
		{
			Slot.ItemActor->Destroy();
		}
		DropItem(Slot, Location);
		OnMicrochipSlotCleared.Broadcast(Index);
	}
}

FVector UInventoryComponent::GetDropLocation_Implementation()
{
	return FVector::ZeroVector;
}

void UInventoryComponent::DropItem_Implementation(FInventorySlot& Slot, FVector Location)
{
	if(IsValid(Slot.CurrentItem))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = OwnerController->GetPawn();
		SpawnParams.Owner = OwnerController;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		if(AItemActor* ActorItem = GetWorld()->SpawnActor<AItemActor>(Slot.CurrentItem->ActorClass, SpawnParams))
		{
			ActorItem->InitializeItemOwner(true);

			if(Slot.CurrentItem->bIsLimitedPower)
			{
				ActorItem->ItemOwner->MaxPower = Slot.CurrentItem->MaxPower;
				ActorItem->ItemOwner->CurrentPower = Slot.CurrentItem->CurrentPower;
				ActorItem->ItemOwner->bIsBurnOut = Slot.CurrentItem->bIsBurnOut;
				ActorItem->ItemOwner->bIsBurnOutPower = Slot.CurrentItem->bIsBurnOutPower;
			}
			
			const FVector2D RandLocation = FMath::RandPointInCircle(DropRandomCircleRadius);
			ActorItem->SetActorLocation(Location + FVector(RandLocation.X, RandLocation.Y, 0.f));
			ActorItem->SetActorRotation(FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f));
			
			Slot.CurrentItem = nullptr;
			Slot.CurrentItemClass = nullptr;
		}
	}
}

