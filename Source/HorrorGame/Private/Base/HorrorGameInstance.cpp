


#include "Base/HorrorGameInstance.h"

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
