


#include "Building/TraitGeneticAlgorithm.h"

void UTraitGeneticAlgorithm::InitializeByRoomType(ERoomTypeEnum NewRoomType)
{
	Type = NewRoomType;
}

void UTraitGeneticAlgorithm::Initialize()
{
}

void UTraitGeneticAlgorithm::Execute(TArray<URoomTraits*>& OutTraits, int32 BestCount)
{
	//OutTTrees.Empty(BestCount);
}
