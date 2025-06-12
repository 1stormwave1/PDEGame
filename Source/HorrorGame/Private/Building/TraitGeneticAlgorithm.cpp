


#include "Building/TraitGeneticAlgorithm.h"

void UTraitGeneticAlgorithm::InitializeByTrait(ETraitEnum NewTrait)
{
	Trait = NewTrait;
}

void UTraitGeneticAlgorithm::Initialize()
{
}

void UTraitGeneticAlgorithm::Execute(TArray<URoomTraitTree*>& OutTTrees, int32 BestCount)
{
	//OutTTrees.Empty(BestCount);
}
