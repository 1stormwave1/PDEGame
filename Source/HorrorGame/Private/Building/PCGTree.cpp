


#include "Building/PCGTree.h"

#include "Building/PCGNode.h"

void UPCGTree::Initialize()
{
	Root = NewObject<UPCGNode>(this, NodeClass);
}
