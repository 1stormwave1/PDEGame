


#include "Building/RoomTraitTree.h"

#include "Kismet/KismetMathLibrary.h"

FTTNode::FTTNode(ETraitEnum NewTrait)
{
	Trait = NewTrait;
}

FTTNode::FTTNode()
{
}

void URoomTraits::InitializeDefault_Implementation()
{
	Traits.Empty();
	
	for(const FTraitSpawnChance& Trait : DefaultTraitsSpawnChance)
	{
		if(UKismetMathLibrary::RandomBoolWithWeight(Trait.SpawnChance))
		{
			Traits.Add(Trait.Node);
		}
	}
}

void URoomTraits::InitializeByRoomType_Implementation(ERoomTypeEnum NewRoomType)
{
	RoomType = NewRoomType;
}
