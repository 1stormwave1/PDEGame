


#include "DataAssets/PDABSPTree.h"

void UPDABSPTree::RetrieveRooms()
{
	if(Tree == nullptr)
	{
		Tree = NewObject<UBSPTree>(this, TreeClass);
	}
	Tree->GetLeaves(RoomsData);
}
