

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Story/Storyline.h"
#include "DialogueComponent.generated.h"


class ANPCController;
class UBehaviorTree;
class AAIController;
class USoundCue;
class UBlackboardComponent;

USTRUCT(BlueprintType)
struct FDialogueStepData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine="true"))
	FText Line;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Responses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundCue> Sound;
};

USTRUCT(BlueprintType)
struct FDialogueStartData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSkippable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPersistent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OwnerName;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStartedSignature, FDialogueStartData, DialogueData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueSkipSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEndedSignature, bool, bTerminate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueNextStepSignature, FDialogueStepData, DialogueStepData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueResponseReceivedSignature, const FText&, Response);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Transient)
	TArray<UStoryline*> ParticipatedStorylines;

	UPROPERTY(BlueprintReadWrite, Transient)
	UBehaviorTree* CurrentDialogueBehaviourTree = nullptr;

	UPROPERTY(BlueprintReadWrite)
	int CurrentStorylineIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	FString DialogueOwnerName;

	UPROPERTY(Transient)
	TObjectPtr<ANPCController> CurrentController = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UBlackboardComponent> CurrentBlackboard = nullptr;

	UPROPERTY(EditAnywhere)
	FName ResponseKeyName = TEXT("Response");

	UPROPERTY(EditAnywhere)
	FName IsNextStepReadyKeyName = TEXT("bIsNextStepReady");

	UPROPERTY(EditAnywhere, Transient)
	UBehaviorTree* PersistentDialogueBehaviourTree = nullptr;
 
public:
	UPROPERTY(BlueprintAssignable)
	FOnDialogueStartedSignature OnDialogueStarted;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueEndedSignature OnDialogueEnded;
	
	UPROPERTY(BlueprintAssignable)
	FOnDialogueNextStepSignature OnDialogueNextStep;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueResponseReceivedSignature OnDialogueResponseReceived;
	
	UDialogueComponent();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartDialogue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NextStepDialogue(FDialogueStepData DialogueStepData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndDialogue(bool bTerminate);

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool IsDialogueActive();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReceiveResponse(const FText& Response);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SkipCurrentDialogue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateCurrentDialogue();
};