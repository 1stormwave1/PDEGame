


#include "Building\PCGRoom.h"
#include "PCGComponent.h"
#include "Components/SplineComponent.h"
#include "Building/BuildingGameState.h"
#include "Building/PCGBuildingSettings.h"
#include "Components/BoxComponent.h"


APCGRoom::APCGRoom()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);

	PCGComponent = CreateDefaultSubobject<UPCGComponent>(TEXT("PCGComponent"));
}

void APCGRoom::BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
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

void APCGRoom::ApplySettings(const UPCGBuildingSettings* Settings)
{
	
}

void APCGRoom::BeginPlay()
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
				BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APCGRoom::BeginOverlap);
			}
		}
	}
}

