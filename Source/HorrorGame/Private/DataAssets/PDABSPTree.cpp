


#include "DataAssets/PDABSPTree.h"

void UPDABSPTree::RetrieveRooms()
{
	if(Tree != nullptr)
	{
		Tree->GetLeaves(RoomsData);
	}
}

void UPDABSPTree::InitializeTree()
{
	if(Tree == nullptr)
	{
		Tree = NewObject<UBSPTree>(this, TreeClass);
		RoomsData.Empty();
	}
}

void UPDABSPTree::InitializeRandomTree(int32 RoomsCount, int32 AvailableZonesCount)
{
	if(Tree == nullptr)
	{
		Tree = NewObject<UBSPTree>(this, TreeClass);
		RoomsData.Empty();
	}
	Tree->InitializeRandom(RoomsCount, AvailableZonesCount);
}
