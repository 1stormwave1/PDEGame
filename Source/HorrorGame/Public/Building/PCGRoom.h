

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class UPCGSettings;
class UBoxComponent;

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
class HORRORGAME_API ARoom : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TArray<UBoxComponent*> RoomZones;

	UPROPERTY(EditAnywhere)
	FName RoomZoneComponentTag = TEXT("RoomZone");

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> InRoomActorClasses;
	
public:
	UPROPERTY(EditAnywhere)
	ERoomType Type;
	
	ARoom();

	UFUNCTION(BlueprintNativeEvent)
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplySettings(const UPCGSettings* Settings);

protected:
	virtual void BeginPlay() override;
};
