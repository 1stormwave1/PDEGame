


#include "Base/Components/DialogueComponent.h"

#include "NPC/NPCController.h"
#include "BrainComponent.h"
#include "Base/HorrorGameInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/NPCCharacter.h"

UDialogueComponent::UDialogueComponent()
{
}


void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentController = GetOwner<ANPCController>();

	/*UHorrorGameInstance* GameInstance =
		Cast<UHorrorGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if(IsValid(CurrentController) && IsValid(GameInstance))
	{
		if(ANPCCharacter* Character = CurrentController->GetPawn<ANPCCharacter>())
		{
			DialogueOwnerName = Character->NPCName.ToString();
			GameInstance->GetParticipatedStorylines(
				DialogueOwnerName, ParticipatedStorylines);
		}
	}

	UpdateCurrentDialogue();*/
}

void UDialogueComponent::UpdateCurrentDialogue_Implementation()
{
	if(ParticipatedStorylines.IsValidIndex(CurrentStorylineIndex))
	{
		CurrentDialogueBehaviourTree =
			ParticipatedStorylines[CurrentStorylineIndex]->GetDialogueTreeByName(DialogueOwnerName);
	}
	if(!IsValid(CurrentDialogueBehaviourTree) && IsValid(PersistentDialogueBehaviourTree))
	{
		CurrentDialogueBehaviourTree = PersistentDialogueBehaviourTree;
	}

	OnDialogueUpdated.Broadcast(CurrentDialogueBehaviourTree);
}

void UDialogueComponent::SkipCurrentDialogue_Implementation()
{
	EndDialogue(true);
}

bool UDialogueComponent::IsDialogueActive_Implementation()
{
	if(CurrentController != nullptr && CurrentController->GetBrainComponent() != nullptr)
	{
		return CurrentController->GetBrainComponent()->IsRunning();
	}
	return false;
}

void UDialogueComponent::NextStepDialogue_Implementation(FDialogueStepData DialogueStepData)
{
	if(CurrentBlackboard != nullptr)
	{
		CurrentBlackboard->SetValueAsBool(IsNextStepReadyKeyName, false);
	}
	if(CurrentController != nullptr)
	{
		if(ACharacter* Character = CurrentController->GetCharacter())
		{
			if(DialogueStepData.AnimMontage != nullptr)
			{
				Character->GetMesh()->GetAnimInstance()->Montage_Play(DialogueStepData.AnimMontage);
			}
		}
	}
	OnDialogueNextStep.Broadcast(DialogueStepData);
}

void UDialogueComponent::StartDialogue_Implementation()
{
	CurrentController = GetOwner<ANPCController>();
	
	if(CurrentController != nullptr)
	{
		if(CurrentController->GetBrainComponent() != nullptr && CurrentController->GetBrainComponent()->IsRunning())
		{
			CurrentController->GetBrainComponent()->RestartLogic();
		}
		CurrentController->RunBehaviorTree(CurrentDialogueBehaviourTree);
		CurrentController->SetFocus(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		CurrentBlackboard = CurrentController->GetBlackboardComponent();
	}

	FDialogueStartData DialogueStartData;
	DialogueStartData.OwnerName = DialogueOwnerName;
	if(CurrentDialogueBehaviourTree == ParticipatedStorylines[CurrentStorylineIndex]->GetDialogueTreeByName(DialogueOwnerName))
	{
		DialogueStartData.bIsSkippable =
			ParticipatedStorylines[CurrentStorylineIndex]->IsCurrentStepDialogueSkippableByOwnerName(DialogueOwnerName);
	}
	DialogueStartData.bIsPersistent = CurrentDialogueBehaviourTree == PersistentDialogueBehaviourTree;

	ParticipatedStorylines[CurrentStorylineIndex]->ContinueStoryline();

	OnDialogueStarted.Broadcast(DialogueStartData);
}



void UDialogueComponent::EndDialogue_Implementation(bool bTerminate)
{
	if(CurrentController != nullptr && CurrentController->GetBrainComponent() != nullptr)
	{
		if(CurrentController->GetBrainComponent()->IsRunning())
		{
			CurrentController->GetBrainComponent()->StopLogic(TEXT("DialogueEnd"));
			CurrentController->ClearFocus(EAIFocusPriority::Gameplay);
			OnDialogueEnded.Broadcast(bTerminate);
		}
	}
	if(ParticipatedStorylines[CurrentStorylineIndex]->GetDialogueTreeByName(DialogueOwnerName) == CurrentDialogueBehaviourTree)
	{
		ParticipatedStorylines[CurrentStorylineIndex]->SetCurrentStepDialogueDoneByOwnerName(DialogueOwnerName);
	}
	UpdateCurrentDialogue();
}

void UDialogueComponent::ReceiveResponse_Implementation(const FText& Response)
{
	if(IsValid(CurrentBlackboard))
	{
		CurrentBlackboard->SetValueAsBool(IsNextStepReadyKeyName, true);
		CurrentBlackboard->SetValueAsString(ResponseKeyName, Response.ToString());

		OnDialogueResponseReceived.Broadcast(Response);
	}
}