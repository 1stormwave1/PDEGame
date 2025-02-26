

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Story/Storyline.h"
#include "HorrorGameInstance.generated.h"


class UBehaviorTree;
class UStoryline;

UCLASS()
class HORRORGAME_API UHorrorGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UStoryline>> StorylineClasses;

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	TArray<UStoryline*> Storylines;
	
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitStory(bool bAllowReInit);

	UFUNCTION(BlueprintPure)
	UStoryline* GetStorylineByName(const FString& StorylineName);

	UFUNCTION(BlueprintCallable)
	void GetBehaviourTreeForCurrentStorylines(const FString& DialogueOwnerName, TArray<UBehaviorTree*>& BehaviourTrees);

	UFUNCTION(BlueprintPure)
	void GetDialoguesForCurrentStorylines(const FString& DialogueOwnerName, TArray<FStorylineDialogue>& OutDialogues);

	UFUNCTION(BlueprintCallable)
	void GetParticipatedStorylines(const FString& DialogueOwnerName, TArray<UStoryline*>& OutStorylines);
};
