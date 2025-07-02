


#include "Building/RoomTraitTree.h"

FTTNode::FTTNode(ETraitEnum NewTrait)
{
	Trait = NewTrait;
}

FTTNode::FTTNode()
{
}

void URoomTraits::InitializeByRoomType_Implementation(ERoomTypeEnum NewRoomType)
{
	RoomType = NewRoomType;
}
