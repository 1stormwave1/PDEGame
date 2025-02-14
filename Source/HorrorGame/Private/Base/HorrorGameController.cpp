


#include "Base/HorrorGameController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Base/HorrorGameCharacter.h"
#include "Base/Components/InteractionComponent.h"
#include "Base/Components/InventoryComponent.h"
#include "Components/ArrowComponent.h"
#include "Inventory/InventoryMainSlotInput.h"
#include "Inventory/ItemActor.h"
#include "UI/HorrorHUD.h"

void AHorrorGameController::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(InventoryComponent))
	{
		InventoryComponent->SetOwnerController(this);
		InventoryComponent->OnActiveSlotIndexChanged.AddDynamic(this, &AHorrorGameController::SetupItemActor);
		InventoryComponent->OnActiveItemChanged.AddDynamic(this, &AHorrorGameController::SetupItemActor);
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InventoryInteractionContext, 0);
	}
}

void AHorrorGameController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		for(int32 i = 0; i < InventoryComponent->MainSlots.Num(); ++i)
		{
			if(InventoryComponent->MainSlots[i].InputAction != nullptr)
			{
				UInventoryMainSlotInput* InventoryMainSlotInput = NewObject<UInventoryMainSlotInput>();
				InventoryMainSlotInput->InventoryComponent = InventoryComponent;
				InventoryMainSlotInput->SlotIndexToActivate = i;
				InventoryMainSlotInputs.Add(InventoryMainSlotInput);

				EnhancedInputComponent->BindAction(
					InventoryComponent->MainSlots[i].InputAction,
					ETriggerEvent::Started, InventoryMainSlotInput, &UInventoryMainSlotInput::ActivateSlot);
			}
		}
		EnhancedInputComponent->BindAction(
			ScrollInputAction, ETriggerEvent::Triggered, this, &AHorrorGameController::ScrollActiveSlot);
		EnhancedInputComponent->BindAction(
			UseItemInputAction, ETriggerEvent::Started, this, &AHorrorGameController::OnItemStartUse);
		EnhancedInputComponent->BindAction(
			UseItemInputAction, ETriggerEvent::Ongoing, this, &AHorrorGameController::OnItemUsing);
		EnhancedInputComponent->BindAction(
			UseItemInputAction, ETriggerEvent::Completed, this, &AHorrorGameController::OnItemFinishUse);
		EnhancedInputComponent->BindAction(
			InventoryInputAction, ETriggerEvent::Started, this, &AHorrorGameController::ToggleInventory);
		EnhancedInputComponent->BindAction(
			PickUpInputAction, ETriggerEvent::Triggered, this, &AHorrorGameController::PickUp);
		EnhancedInputComponent->BindAction(
					PauseAction, ETriggerEvent::Triggered, this, &AHorrorGameController::PauseGame);
	}
}

AHorrorGameController::AHorrorGameController()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AHorrorGameController::SetupItemActor_Implementation(int32 NewCurrentActiveSlotIndex)
{
	if(AHorrorGameCharacter* HorrorCharacter = GetPawn<AHorrorGameCharacter>())
	{
		InventoryComponent->DestroyActiveSlotItem();
		InventoryComponent->SpawnActiveSlotItem(HorrorCharacter->GetItemAttachPointComponent());
	}
}

void AHorrorGameController::ScrollActiveSlot(const FInputActionValue& Value)
{
	const float ScrollValue = Value.Get<float>();
	if(ScrollValue > 0.f)
	{
		InventoryComponent->SetActiveSlotIndex(InventoryComponent->GetCurrentActiveSlotIndex() + 1);
	}
	else if(ScrollValue < 0.f)
	{
		InventoryComponent->SetActiveSlotIndex(InventoryComponent->GetCurrentActiveSlotIndex() - 1);
	}
}

void AHorrorGameController::OnItemStartUse()
{
	AItemActor* CurrentItemActor = InventoryComponent->GetCurrentItemActor();
	if(IsValid(CurrentItemActor))
	{
		if(UBodyItem* BodyItem = Cast<UBodyItem>(InventoryComponent->GetCurrentActiveItem()))
		{
			if(!InventoryComponent->PutItemInBodySlotByTypeWithActor(CurrentItemActor, BodyItem, BodyItem->BodyType))
			{
				return;
			}
			InventoryComponent->SetCurrentItemActor(nullptr);
			InventoryComponent->ClearQASlot(InventoryComponent->GetCurrentActiveSlotIndex());
		}
		CurrentItemActor->StartUseItem();
	}
}

void AHorrorGameController::OnItemUsing()
{
	AItemActor* CurrentItemActor = InventoryComponent->GetCurrentItemActor();
	if(IsValid(CurrentItemActor))
	{
		CurrentItemActor->OnItemUsing();
	}
}

void AHorrorGameController::OnItemFinishUse()
{
	AItemActor* CurrentItemActor = InventoryComponent->GetCurrentItemActor();
	if(IsValid(CurrentItemActor))
	{
		CurrentItemActor->FinishUseItem();
	}
}

void AHorrorGameController::PauseGame()
{
	SetPause(true);
	if(AHorrorHUD* HUD = GetHUD<AHorrorHUD>())
	{
		HUD->SetDisplay(EDisplayName::Pause);
	}
}

void AHorrorGameController::CancelInteract_Implementation()
{
	if(InteractionQueue.IsEmpty())
	{
		return;
	}
	
	if(UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(InteractionQueue.Top()))
	{
		InteractionComponent->CancelInteract();
	}
}

void AHorrorGameController::PickUp_Implementation()
{
	if(InteractionQueue.IsEmpty())
	{
		return;
	}
	
	if(UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(InteractionQueue.Top()))
	{
		InteractionComponent->Interact();
	}
}

void AHorrorGameController::ToggleInventory_Implementation()
{
	OnInventoryToggle.Broadcast();
}
