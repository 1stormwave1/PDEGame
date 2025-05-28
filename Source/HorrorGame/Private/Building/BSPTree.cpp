


#include "Building/BSPTree.h"

void UBSPTree::GetLeaves(TArray<FBSPNode>& OutLeaves)
{
	OutLeaves.Empty((Tree.Num() + 1) / 2);

	for(int32 i = 0; i < Tree.Num(); ++i)
	{
		const int32 Left = 2 * i + 1;
		const int32 Right = 2 * i + 2;

		if(Left > Tree.Num() && Right > Tree.Num())
		{
			OutLeaves.Add(Tree[i]);
		}
	}
}
