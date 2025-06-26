

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"


class UAISenseConfig_Sight;
class UBehaviorTreeComponent;

UCLASS()
class HORRORGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController(const FObjectInitializer& ObjectInitializer);

	UBehaviorTreeComponent* GetBehaviorTreeComponent() const;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI|Perception")
	UAISenseConfig_Sight* Sight = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	UBlackboardData* BlackboardData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	UAIPerceptionComponent* AIPerceptionComponent;
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnActorSensed(AActor* Actor, FAIStimulus Stimulus);
	
};
