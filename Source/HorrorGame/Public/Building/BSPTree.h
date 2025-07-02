

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
	URoomTraits* RoomTraits = nullptr;

	FBSPNode(int32 NewZoneIndex = -1);

	friend bool operator==(const FBSPNode& Lhs, const FBSPNode& RHS)
	{
		return Lhs.ZoneIndex == RHS.ZoneIndex;
	}

	friend bool operator!=(const FBSPNode& Lhs, const FBSPNode& RHS)
	{
		return !(Lhs == RHS);
	}
};


UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UBSPTree : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FBSPNode> Tree;

	UPROPERTY(EditAnywhere)
	int32 Seed = -1;

	FRandomStream Stream;
	
	UFUNCTION(BlueprintCallable)
	void GetLeaves(TArray<FBSPNode>& OutLeaves);

	void DFSFindLeaves(int32 LeftIndex, int32 RightIndex, TArray<FBSPNode>& OutLeaves);

	UFUNCTION(BlueprintCallable)
	void InitializeRandom(int32 RoomsCount, int32 AvailableZonesCount);
	
	void PrescribeRoomIndex(int32 TreeIndex, int32 ZoneIndexToPrescribe, TArray<int32>& AvailableZoneIndexes, int32 LeftIndexMin, int32 RightIndexMax);
};
