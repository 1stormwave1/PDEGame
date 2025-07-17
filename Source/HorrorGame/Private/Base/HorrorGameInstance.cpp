


#include "Base/HorrorGameInstance.h"
#include "Base/MainSaveGame.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Story/Storyline.h"

void UHorrorGameInstance::Init()
{
	Super::Init();

	if(USaveGame* SaveObject = UGameplayStatics::LoadGameFromSlot(MainSaveGameSlotName, 0))
	{
		MainSaveGame = Cast<UMainSaveGame>(SaveObject);
	}

	if(MainSaveGame == nullptr && MainSaveGameClass != nullptr)
	{
		MainSaveGame =
			Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(MainSaveGameClass));
	}
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

void UHorrorGameInstance::AddCollectedItem(int32 Count)
{
	if(MainSaveGame != nullptr)
	{
		MainSaveGame->BuildingItemsCollected += Count;

		//UGameplayStatics::SaveGameToSlot(MainSaveGame, MainSaveGameSlotName, 0);
	}
}

void UHorrorGameInstance::AddCollectableGlobal(int32 Count)
{
	if(MainSaveGame != nullptr)
	{
		MainSaveGame->GlobalItemCollected += Count;

		UGameplayStatics::SaveGameToSlot(MainSaveGame, MainSaveGameSlotName, 0);
	}
}

void UHorrorGameInstance::InitStory_Implementation(bool bAllowReInit)
{
	if(bAllowReInit || Storylines.IsEmpty())
	{
		Storylines.Empty(StorylineClasses.Num());
		for(TSubclassOf<UStoryline> StorylineClass : StorylineClasses)
		{
			UStoryline* MainStory = NewObject<UStoryline>(this, StorylineClass);
			if(MainSaveGame != nullptr)
			{
				MainStory->CurrentStepIndex = MainSaveGame->MainStoryStep;
			}
			Storylines.Add(MainStory);
		}
	}
}
