


#include "Building/RoomZone.h"
#include "Building/BuildingGameState.h"
#include "Components/BoxComponent.h"


ARoomZone::ARoomZone()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
}

void ARoomZone::BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValid(OtherActor))
	{
		return;
	}
	
	if(OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if(ABuildingGameState* GameState = GetWorld()->GetGameState<ABuildingGameState>())
		{
			GameState->OnCurrentRoomChanged(RoomOwner);
		}
	}
}

void ARoomZone::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARoomZone::BeginOverlap);

	if(AActor* ParentActor = GetParentActor())
	{
		if(AActor* AttachedActor = ParentActor->GetAttachParentActor())
		{
			RoomOwner = Cast<APCGRoom>(AttachedActor);
		}
	}
}
