

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class UPCGSettings;
class UBoxComponent;

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	None = 0
};

UCLASS()
class HORRORGAME_API ARoom : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERoomType RoomType;

	UPROPERTY(Transient)
	TArray<UBoxComponent*> RoomZones;

	UPROPERTY(EditAnywhere)
	FName RoomZoneComponentTag = TEXT("RoomZone");

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> InRoomActorClasses;
	
public:
	ARoom();

	void ApplySettings(const UPCGSettings* Settings);

protected:
	virtual void BeginPlay() override;
};
