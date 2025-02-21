

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEndedSignature, bool, bTerminate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueNextStepSignature, FDialogueStepData, DialogueStepData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueResponseReceivedSignature, const FText&, Response);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UBehaviorTree> DialogueTree;

	UPROPERTY(Transient)
	TObjectPtr<ANPCController> CurrentController = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UBlackboardComponent> CurrentBlackboard = nullptr;

	UPROPERTY(EditAnywhere)
	FName ResponseKeyName = TEXT("Response");

	UPROPERTY(EditAnywhere)
	FName IsNextStepReadyKeyName = TEXT("bIsNextStepReady");
 
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
};