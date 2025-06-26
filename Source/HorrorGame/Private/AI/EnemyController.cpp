


#include "AI/EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));
	if(Sight)
	{
		Sight->SightRadius = 500;
		Sight->LoseSightRadius = 600;
		Sight->PeripheralVisionAngleDegrees = 180.0f;
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = true;

		AIPerceptionComponent->ConfigureSense(*Sight);
	}
}

UBehaviorTreeComponent* AEnemyController::GetBehaviorTreeComponent() const
{
	return BehaviorTreeComponent;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnActorSensed);
	if (IsValid(BlackboardData))
	{
		UseBlackboard(BlackboardData, BlackboardComponent);
	}

	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(Blackboard) && IsValid(BehaviorTree))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

void AEnemyController::OnActorSensed_Implementation(AActor* Actor, FAIStimulus Stimulus)
{
}
