


#include "Building\PCGRoom.h"
#include "PCGComponent.h"
#include "Components/SplineComponent.h"


APCGRoom::APCGRoom()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);

	PCGComponent = CreateDefaultSubobject<UPCGComponent>(TEXT("PCGComponent"));
}
void APCGRoom::ApplySettings(const UPCGBuildingSettings* Settings)
{
	
}

void APCGRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

