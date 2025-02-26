


#include "Story/Storyline.h"

UBehaviorTree* UStoryline::GetDialogueTreeByName(const FString& DialogueOwnerName)
{
	if(Steps.IsValidIndex(CurrentStepIndex))
	{
		for(const FStorylineDialogue& Dialogue : Steps[CurrentStepIndex].Dialogues)
		{
			if(Dialogue.DialogueOwnerName == DialogueOwnerName)
			{
				return Dialogue.DialogueTree;
			}
		}
	}
	return nullptr;
}

FStorylineDialogue UStoryline::GetDialogueByName(const FString& DialogueOwnerName)
{
	if(Steps.IsValidIndex(CurrentStepIndex))
	{
		for(FStorylineDialogue& Dialogue : Steps[CurrentStepIndex].Dialogues)
		{
			if(Dialogue.DialogueOwnerName == DialogueOwnerName)
			{
				return Dialogue;
			}
		}
	}
	return FStorylineDialogue();
}

void UStoryline::SetCurrentStepDialogueDoneByOwnerName_Implementation(const FString& DialogueOwnerName)
{
	//no multiple dialogues with the same DialogueOwnerName in the same step!!! 
	if(Steps.IsValidIndex(CurrentStepIndex))
	{
		for(FStorylineDialogue& Dialogue : Steps[CurrentStepIndex].Dialogues)
		{
			if(Dialogue.DialogueOwnerName == DialogueOwnerName)
			{
				Dialogue.bIsDone = true;
			}
		}
		if(IsStorylineCurrentStepDone())
		{
			ContinueStoryline();
		}
	}
}

bool UStoryline::IsCurrentStepDialogueSkippableByOwnerName_Implementation(const FString& DialogueOwnerName)
{
	if(Steps.IsValidIndex(CurrentStepIndex))
	{
		for(FStorylineDialogue& Dialogue : Steps[CurrentStepIndex].Dialogues)
		{
			if(Dialogue.DialogueOwnerName == DialogueOwnerName)
			{
				return Dialogue.bIsSkippable;
			}
		}
	}
	return false;
}

bool UStoryline::IsStorylineCurrentStepDone() const
{
	if(Steps.IsValidIndex(CurrentStepIndex))
	{
		for(const FStorylineDialogue& Dialogue : Steps[CurrentStepIndex].Dialogues)
		{
			if(!Dialogue.bIsDone)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void UStoryline::SetCurrentStepDialogueDone_Implementation(const FString& DialogueName)
{
	if(Steps.IsValidIndex(CurrentStepIndex))
	{
		for(FStorylineDialogue& Dialogue : Steps[CurrentStepIndex].Dialogues)
		{
			if(Dialogue.DialogueName == DialogueName)
			{
				Dialogue.bIsDone = true;
			}
		}
		if(IsStorylineCurrentStepDone())
		{
			ContinueStoryline();
		}
	}
}

void UStoryline::ContinueStoryline_Implementation()
{
	OnStorylineContinued.Broadcast(++CurrentStepIndex);
}
