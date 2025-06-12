

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.h"
#include "BSPTree.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FBSPNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ZoneIndex = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URoomTraitTree* RoomTraitTree = nullptr;

	FBSPNode(int32 NewZoneIndex = -1);
};


UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UBSPTree : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FBSPNode> Tree;
	
	UFUNCTION(BlueprintCallable)
	void GetLeaves(TArray<FBSPNode>& OutLeaves);

	UFUNCTION(BlueprintCallable)
	void InitializeRandom(int32 RoomsCount, int32 AvailableZonesCount);

	void PrescribeRoomIndex(int32 TreeIndex, int32 ZoneIndexToPrescribe, TArray<int32>& AvailableZoneIndexes);
};
