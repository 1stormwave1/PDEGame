


#include "Base/Components/DialogueComponent.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UDialogueComponent::UDialogueComponent()
{
}


void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentController = GetOwner<AAIController>();
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
	if(CurrentController != nullptr)
	{
		if(CurrentController->GetBrainComponent() != nullptr && CurrentController->GetBrainComponent()->IsRunning())
		{
			CurrentController->GetBrainComponent()->RestartLogic();
		}
		CurrentController->RunBehaviorTree(DialogueTree);
		CurrentController->SetFocus(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		CurrentBlackboard = CurrentController->GetBlackboardComponent();
	}
	OnDialogueStarted.Broadcast();
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
}

void UDialogueComponent::ReceiveResponse_Implementation(const FText& Response)
{
	CurrentBlackboard->SetValueAsBool(IsNextStepReadyKeyName, true);
	CurrentBlackboard->SetValueAsString(ResponseKeyName, Response.ToString());

	OnDialogueResponseReceived.Broadcast(Response);
}