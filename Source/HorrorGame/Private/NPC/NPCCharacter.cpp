


#include "NPC/NPCCharacter.h"

#include "Base/HorrorGameInstance.h"
#include "Base/Components/DialogueComponent.h"
#include "Base/Components/InteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/NPCController.h"


ANPCCharacter::ANPCCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetupAttachment(RootComponent);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent->OnInteractChanged.AddDynamic(this, &ANPCCharacter::OnInteractChanged);
	InteractionComponent->OnInteracted.AddDynamic(this, &ANPCCharacter::OnInteract);
	InteractionComponent->OnInteractCanceled.AddDynamic(this, &ANPCCharacter::OnInteractCancel);

	UHorrorGameInstance* GameInstance =
		Cast<UHorrorGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	const ANPCController* CurrentController = GetController<ANPCController>();
	
	if(IsValid(GameInstance) && IsValid(CurrentController))
	{
		UDialogueComponent* DialogueComponent = CurrentController->DialogueComponent;
		
		DialogueComponent->DialogueOwnerName = NPCName.ToString();
		GameInstance->GetParticipatedStorylines(
			DialogueComponent->DialogueOwnerName, DialogueComponent->ParticipatedStorylines);
		DialogueComponent->UpdateCurrentDialogue();
	}
}

void ANPCCharacter::OnInteract_Implementation()
{
	if(ANPCController* NPCController = GetController<ANPCController>())
	{
		NPCController->DialogueComponent->StartDialogue();
	}
}

void ANPCCharacter::OnInteractCancel_Implementation()
{
	if(ANPCController* NPCController = GetController<ANPCController>())
	{
		NPCController->DialogueComponent->EndDialogue(true);
	}
}

void ANPCCharacter::OnInteractChanged_Implementation(bool bIsInteractable)
{
	InteractionWidgetComponent->SetHiddenInGame(!bIsInteractable);
}
