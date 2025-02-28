


#include "Building/PCGTree.h"
#include "Building/PCGBuildingNode.h"

void UPCGTree::Initialize()
{
	Root = NewObject<UPCGBuildingNode>(this, NodeClass);
}
