


#include "Building/Room.h"

#include "Components/BoxComponent.h"


ARoom::ARoom()
{
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
			}
		}
	}
}

