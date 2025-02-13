


#include "Inventory/ItemActor.h"

#include "Base/Components/InteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "Inventory/Item.h"


void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeItemOwner(bInitializeItemOwner);
	
	InteractionComponent->OnInteractChanged.AddDynamic(this, &AItemActor::OnInteractabilityChanged);
	InteractionWidgetComponent->SetHiddenInGame(true);
}

AItemActor::AItemActor()
{
	OffsetComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OffsetComponent"));
	OffsetComponent->SetupAttachment(RootComponent);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OffsetComponent);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetupAttachment(OffsetComponent);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(OffsetComponent);
}

void AItemActor::InitializeItemOwner(bool bNewInitializeItemOwner)
{
	bInitializeItemOwner = bNewInitializeItemOwner;
	if(bInitializeItemOwner && DefaultItemClass != nullptr)
	{
		ItemOwner = NewObject<UItem>(this, DefaultItemClass);
		InteractionComponent->ItemOwner = ItemOwner;
	}
}

void AItemActor::OnInteractabilityChanged_Implementation(bool bIsInteractable)
{
	InteractionWidgetComponent->SetHiddenInGame(!bIsInteractable);
}

void AItemActor::SetVisibility_Implementation(bool bNewVisibility)
{
}

void AItemActor::SetItemActorParams_Implementation(UItem* Item)
{
	ItemOwner = Item;
	InteractionComponent->ItemOwner = ItemOwner;
}

void AItemActor::StartUseItem_Implementation()
{
}

void AItemActor::FinishUseItem_Implementation()
{
}

void AItemActor::OnItemUsing_Implementation()
{
}

