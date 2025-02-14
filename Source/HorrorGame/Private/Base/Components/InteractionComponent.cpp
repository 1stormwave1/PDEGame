
#include "Base/Components/InteractionComponent.h"

#include "Base/HorrorGameController.h"
#include "Base/Components/InventoryComponent.h"
#include "Inventory/ItemActor.h"


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	MainController = GetWorld()->GetFirstPlayerController<AHorrorGameController>();

	if(const AItemActor* ItemActor = GetOwner<AItemActor>())
	{
		ItemOwner = ItemActor->ItemOwner;
	}
	
	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::BeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::EndOverlap);
	
	CheckOverlap(MainController->GetPawn());
}

UInteractionComponent::UInteractionComponent()
{
	SphereRadius = 150.f;
}

void UInteractionComponent::CancelInteract_Implementation()
{
	OnInteractCanceled.Broadcast();
}

void UInteractionComponent::CheckOverlap_Implementation(AActor* OtherActor)
{
	if(MainController->GetInventoryComponent()->DoesContainItem(ItemOwner))
	{
		return;
	}

	if(!IsValid(MainController))
	{
		return;
	}
	
	if(IsOverlappingActor(OtherActor) && OtherActor == MainController->GetPawn())
	{
		MainController->InteractionQueue.Push(this);
		bIsOverlapping = true;
		OnInteractChanged.Broadcast(bIsOverlapping);
	}
}

void UInteractionComponent::EndOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(MainController->GetInventoryComponent()->DoesContainItem(ItemOwner))
	{
		return;
	}
	
	if(!IsValid(MainController))
	{
		return;
	}
	
	if(OtherActor == MainController->GetPawn())
	{
		MainController->InteractionQueue.Remove(this);
		bIsOverlapping = false;
		OnInteractChanged.Broadcast(bIsOverlapping);
	}
}

void UInteractionComponent::BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckOverlap(OtherActor);
}

void UInteractionComponent::Interact_Implementation()
{
	if(!IsValid(MainController))
	{
		return;
	}

	if(IsValid(ItemOwner))
	{
		if(MainController->GetInventoryComponent()->PickUpItem(ItemOwner))
		{
			MainController->InteractionQueue.Remove(this);
			bIsOverlapping = false;
			
			GetOwner()->Destroy();
		}
	}

	OnInteracted.Broadcast();
}