

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PDABSPTree.h"
#include "GameFramework/Actor.h"
#include "PCGRoom.generated.h"

class UPCGBuildingSettings;
class UBoxComponent;
class USplineComponent;
class UPCGComponent;

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	None = 0,
	Common,
	Treasure,
	Storage,
	Laboratory,
	Testing,
	Bedroom,
	Canteen
};

UCLASS()
class HORRORGAME_API APCGRoom : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TArray<UBoxComponent*> RoomZones;

	UPROPERTY(EditAnywhere)
	FName RoomZoneComponentTag = TEXT("RoomZone");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPCGComponent> PCGComponent;
	
public:
	UPROPERTY(EditAnywhere)
	ERoomType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallWidth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallHeight = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallThickness = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Subdivisions = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Seed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPDABSPTree* BSPTreeData = nullptr;
	
	APCGRoom();

	void ApplySettings(const UPCGBuildingSettings* Settings);

protected:
	virtual void BeginPlay() override;
};
