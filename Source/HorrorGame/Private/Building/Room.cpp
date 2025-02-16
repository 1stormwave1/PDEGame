


#include "Building/Room.h"

#include "Building/BuildingGameState.h"
#include "Components/BoxComponent.h"


ARoom::ARoom()
{
}

void ARoom::BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if(ABuildingGameState* GameState = GetWorld()->GetGameState<ABuildingGameState>())
		{
			GameState->OnCurrentRoomChanged(this);
		}
	}
}

void ARoom::ApplySettings(const UPCGSettings* Settings)
{
	
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> RoomChildren;
	RootComponent->GetChildrenComponents(true, RoomChildren);
	for(USceneComponent* RoomChild : RoomChildren)
	{
		if(UBoxComponent* BoxComponent = Cast<UBoxComponent>(RoomChild))
		{
			if(BoxComponent->ComponentHasTag(RoomZoneComponentTag))
			{
				RoomZones.Add(BoxComponent);
				BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARoom::BeginOverlap);
			}
		}
	}
}

