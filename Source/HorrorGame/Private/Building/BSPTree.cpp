


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
	
	const int32 NodesCount = 2 * RoomsCount - 1;
	Tree.Empty(NodesCount);
	Tree.SetNum(NodesCount);

	if(Seed >= 0)
	{
		Stream.Initialize(Seed);
	}
	else
	{
		Stream.Initialize(NAME_None);
	}
	
	const int32 ZoneIndexToPrescribe = AvailableZoneIndexes[Stream.RandRange(0, AvailableZoneIndexes.Num() - 1)];
	PrescribeRoomIndex(0, ZoneIndexToPrescribe, AvailableZoneIndexes, 0, AvailableZoneIndexes.Num() - 1);
}

void UBSPTree::PrescribeRoomIndex(int32 TreeIndex, int32 ZoneIndexToPrescribe, TArray<int32>& AvailableZoneIndexes, int32 LeftIndexMin, int32 RightIndexMax)
{
	if(!Tree.IsValidIndex(TreeIndex))
	{
		return;
	}

	Tree[TreeIndex].ZoneIndex = ZoneIndexToPrescribe;
	
	const int32 LeftTreeIndex = 2 * TreeIndex + 1;
	const int32 RightTreeIndex = 2 * TreeIndex + 2;

	if(!Tree.IsValidIndex(LeftTreeIndex))
	{
		return;
	}
	if(!Tree.IsValidIndex(RightTreeIndex))
	{
		return;
	}

	if(!AvailableZoneIndexes.IsEmpty())
	{
		const int32 SeparatingIndex = (LeftIndexMin + RightIndexMax) / 2;
		const bool bIsNewIndexLeft = ZoneIndexToPrescribe >= SeparatingIndex;

		const int32 TreeIndexToPrescribeNewZoneIndex = bIsNewIndexLeft ? LeftTreeIndex : RightTreeIndex;
		const int32 AnotherTreeIndex = bIsNewIndexLeft ? RightTreeIndex : LeftTreeIndex;

		int32 NewRandomIndex = -1;
		while (!AvailableZoneIndexes.Contains(NewRandomIndex))
		{
			NewRandomIndex = bIsNewIndexLeft ?
			Stream.RandRange(LeftIndexMin, SeparatingIndex) :
			Stream.RandRange(SeparatingIndex + 1, RightIndexMax);
		}
		AvailableZoneIndexes.Remove(NewRandomIndex);

		if(bIsNewIndexLeft)
		{
			PrescribeRoomIndex(TreeIndexToPrescribeNewZoneIndex, NewRandomIndex, AvailableZoneIndexes, LeftIndexMin, SeparatingIndex);
			PrescribeRoomIndex(AnotherTreeIndex, ZoneIndexToPrescribe, AvailableZoneIndexes, SeparatingIndex + 1, RightIndexMax);
		}
		else
		{
			PrescribeRoomIndex(TreeIndexToPrescribeNewZoneIndex, NewRandomIndex, AvailableZoneIndexes, SeparatingIndex + 1, RightIndexMax);
			PrescribeRoomIndex(AnotherTreeIndex, ZoneIndexToPrescribe, AvailableZoneIndexes, LeftIndexMin, SeparatingIndex);
		}
	}
}
