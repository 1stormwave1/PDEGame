

#pragma once

#include "CoreMinimal.h"
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
	float WallSize = 100.f;
	
	APCGRoom();

	UFUNCTION(BlueprintNativeEvent)
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplySettings(const UPCGBuildingSettings* Settings);

protected:
	virtual void BeginPlay() override;
};
