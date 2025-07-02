

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Building/PCGRoom.h"
#include "PDAModularRoom.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FRoomComponentData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOpenBack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale = FVector(1.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset;
};

USTRUCT(Blueprintable, BlueprintType)
struct FRoomFloorClassByTraits
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> FloorActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomTypeEnum RoomType;
};

UCLASS()
class HORRORGAME_API UPDAModularRoom : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoomComponentData> RoomWalls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoomComponentData> RoomPillars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor")
	TArray<FRoomComponentData> RoomFloors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor")
	TArray<FRoomFloorClassByTraits> RoomFloorClassesByTraits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoomComponentData> RoomCeilings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoomComponentData> Doors;
};
