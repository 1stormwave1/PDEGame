


#include "Building/BSPTree.h"


FBSPNode::FBSPNode(int32 NewZoneIndex)
{
	ZoneIndex = NewZoneIndex;
}

void UBSPTree::GetLeaves(TArray<FBSPNode>& OutLeaves)
{
	OutLeaves.Empty((Tree.Num() + 1) / 2);

	for(int32 i = 0; i < Tree.Num(); ++i)
	{
		const int32 Left = 2 * i + 1;
		const int32 Right = 2 * i + 2;

		if(Left >= Tree.Num() && Right >= Tree.Num())
		{
			OutLeaves.Add(Tree[i]);
		}
	}
}

void UBSPTree::InitializeRandom(int32 RoomsCount, int32 AvailableZonesCount)
{
	if(RoomsCount > AvailableZonesCount)
	{
		return;
	}
	
	TArray<int32> AvailableZoneIndexes;
	for(int32 i = 0; i < AvailableZonesCount; ++i)
	{
		AvailableZoneIndexes.Add(i);
	}
	
	TArray<int32> AvailableIndexes;
	for(int32 i = 0; i < RoomsCount; ++i)
	{
		const int32 AddedIndex = AvailableZoneIndexes[FMath::RandRange(0, AvailableZoneIndexes.Num() - 1)];

		AvailableIndexes.Add(AddedIndex);
		AvailableZoneIndexes.Remove(AddedIndex);
	}
	
	const int32 NodesCount = 2 * RoomsCount - 1;
	Tree.Empty(NodesCount);
	Tree.SetNum(NodesCount);
	
	const int32 AvailableRandomIndex = FMath::RandRange(0, AvailableIndexes.Num() - 1);
	const int32 ZoneRandomIndex = AvailableIndexes[AvailableRandomIndex];
	PrescribeRoomIndex(0, ZoneRandomIndex, AvailableIndexes);
}

void UBSPTree::PrescribeRoomIndex(int32 TreeIndex, int32 ZoneIndexToPrescribe, TArray<int32>& AvailableZoneIndexes)
{
	if(!Tree.IsValidIndex(TreeIndex))
	{
		return;
	}
	
	Tree[TreeIndex].ZoneIndex = ZoneIndexToPrescribe;
	AvailableZoneIndexes.Remove(ZoneIndexToPrescribe);

	const int32 LeftTreeIndex = 2 * TreeIndex + 1;
	const int32 RightTreeIndex = 2 * TreeIndex + 2;

	if(!AvailableZoneIndexes.IsEmpty())
	{
		const int32 TreeIndexToPrescribeNewZoneIndex = FMath::RandBool() ? LeftTreeIndex : RightTreeIndex;
		const int32 AnotherTreeIndex = TreeIndexToPrescribeNewZoneIndex == LeftTreeIndex ? RightTreeIndex : LeftTreeIndex;
		const int32 AvailableRandomIndex = FMath::RandRange(0, AvailableZoneIndexes.Num() - 1);
		const int32 ZoneRandomIndex = AvailableZoneIndexes[AvailableRandomIndex];

		PrescribeRoomIndex(TreeIndexToPrescribeNewZoneIndex, ZoneRandomIndex, AvailableZoneIndexes);
		PrescribeRoomIndex(AnotherTreeIndex, ZoneIndexToPrescribe, AvailableZoneIndexes);
	}
}
