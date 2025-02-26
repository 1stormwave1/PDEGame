

#pragma once

#include "CoreMinimal.h"
#include "Storyline.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStorylineContinuedSignature, int, StepIndex);

USTRUCT(BlueprintType)
struct FStorylineDialogue
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DialogueOwnerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DialogueName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsSkippable = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDone = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UBehaviorTree> DialogueTree;
	
	FStorylineDialogue() = default;
};


USTRUCT(BlueprintType)
struct FStorylineStep
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FStorylineDialogue> Dialogues;
};

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UStoryline : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	int CurrentStepIndex = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStorylineStep> Steps;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StorylineName;
	
	UFUNCTION(BlueprintPure)
	UBehaviorTree* GetDialogueTreeByName(const FString& DialogueOwnerName);

	UFUNCTION(BlueprintPure)
	FStorylineDialogue GetDialogueByName(const FString& DialogueOwnerName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ContinueStoryline();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCurrentStepDialogueDone(const FString& DialogueName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCurrentStepDialogueDoneByOwnerName(const FString& DialogueOwnerName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCurrentStepDialogueSkippableByOwnerName(const FString& DialogueOwnerName);

	UFUNCTION(BlueprintPure)
	bool IsStorylineCurrentStepDone() const;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnStorylineContinuedSignature OnStorylineContinued;
};