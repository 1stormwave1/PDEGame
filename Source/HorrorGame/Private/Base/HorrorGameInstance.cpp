


#include "Base/HorrorGameInstance.h"

#include "Base/Components/DialogueComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Story/Storyline.h"

void UHorrorGameInstance::Init()
{
	Super::Init();
}

UStoryline* UHorrorGameInstance::GetStorylineByName(const FString& StorylineName)
{
	for(UStoryline* Storyline : Storylines)
	{
		if(IsValid(Storyline) && Storyline->StorylineName == StorylineName)
		{
			return Storyline;
		}
	}
	return nullptr;
}

void UHorrorGameInstance::GetBehaviourTreeForCurrentStorylines(const FString& DialogueOwnerName,
	TArray<UBehaviorTree*>& BehaviourTrees)
{
	for(UStoryline* Storyline : Storylines)
	{
		if(IsValid(Storyline))
		{
			if(UBehaviorTree* BehaviorTree = Storyline->GetDialogueTreeByName(DialogueOwnerName))
			{
				BehaviourTrees.Add(BehaviorTree);
			}
		}
	}
}

void UHorrorGameInstance::GetDialoguesForCurrentStorylines(const FString& DialogueOwnerName,
	TArray<FStorylineDialogue>& OutDialogues)
{
	for(UStoryline* Storyline : Storylines)
	{
		if(IsValid(Storyline))
		{
			FStorylineDialogue Dialogue = Storyline->GetDialogueByName(DialogueOwnerName);
			OutDialogues.Add(Dialogue);
		}
	}
}

void UHorrorGameInstance::GetParticipatedStorylines(const FString& DialogueOwnerName, TArray<UStoryline*>& OutStorylines)
{
	for(UStoryline* Storyline : Storylines)
	{
		if(IsValid(Storyline))
		{
			for(FStorylineStep& Step : Storyline->Steps)
			{
				for(FStorylineDialogue& Dialogue : Step.Dialogues)
				{
					if(Dialogue.DialogueOwnerName == DialogueOwnerName)
					{
						OutStorylines.AddUnique(Storyline);
						break;
					}
				}
			}
		}
	}
}

void UHorrorGameInstance::InitStory_Implementation(bool bAllowReInit)
{
	if(bAllowReInit || Storylines.IsEmpty())
	{
		Storylines.Empty(StorylineClasses.Num());
		for(TSubclassOf<UStoryline> StorylineClass : StorylineClasses)
		{
			Storylines.Add(NewObject<UStoryline>(this, StorylineClass));
		}
	}
}
