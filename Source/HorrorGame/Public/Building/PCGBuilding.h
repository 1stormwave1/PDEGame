

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCGBuilding.generated.h"

class USplineComponent;
class UPCGComponent;

UCLASS()
class HORRORGAME_API APCGBuilding : public AActor
{
	GENERATED_BODY()
	
public:
	APCGBuilding();

protected:
	virtual void BeginPlay() override;
};
