


#include "Building/TraitGeneticAlgorithm.h"

#include "Algo/RandomShuffle.h"
#include "Building/GeneticAlgorithmSave.h"

void UTraitGeneticAlgorithm::InitializeByRoomType(ERoomTypeEnum NewRoomType)
{
	Type = NewRoomType;
}

void UTraitGeneticAlgorithm::Initialize(UGeneticAlgorithmSave* NewGeneticAlgorithmSave, int32 MinPopulationCount)
{
	if(!IsValid(NewGeneticAlgorithmSave))
	{
		return;
	}

	GeneticAlgorithmSave = NewGeneticAlgorithmSave;

	TArray<FSavedRoomTraits> SavedRoomTraits;
	GeneticAlgorithmSave->GetTraitsByRoomType(SavedRoomTraits, Type);

	for(const FSavedRoomTraits& Traits : SavedRoomTraits)
	{
		URoomTraits* NewRoomTraits = NewObject<URoomTraits>(this, RoomTraitsClass);
		NewRoomTraits->RoomType = Traits.RoomType;
		NewRoomTraits->Traits = Traits.RoomTraits;
		
		TraitsPopulation.Add(NewRoomTraits);
	}

	if(TraitsPopulation.Num() < MinPopulationCount)
	{
		const int32 AdditionalTraitsCount = MinPopulationCount - TraitsPopulation.Num();
		for(int32 i = 0; i < AdditionalTraitsCount; ++i)
		{
			URoomTraits* NewRoomTraits = NewObject<URoomTraits>(this, RoomTraitsClass);
			NewRoomTraits->InitializeDefault();

			TraitsPopulation.Add(NewRoomTraits);
		}
	}
	else
	{
		//+1 залежно від типу гравця (після аналізу даних)
		URoomTraits* NewRoomTraits = NewObject<URoomTraits>(this, RoomTraitsClass);
		NewRoomTraits->InitializeDefault();

		TraitsPopulation.Add(NewRoomTraits);
	}
}

void UTraitGeneticAlgorithm::Execute(TArray<URoomTraits*>& OutTraits, int32 BestCount)
{
	if(TraitsPopulation.Num() == BestCount)
	{
		for(int32 i = 0; i < BestCount; ++i)
		{
			OutTraits.Add(TraitsPopulation[i]);
			GeneticAlgorithmSave->AddRoomTraitsToSave(TraitsPopulation[i]);
		}
		return;
	}

	TArray<URoomTraits*> NewPopulation;
	GetSortedPopulation(NewPopulation);

	const int32 SelectedPopulationCount = FMath::RoundToInt32(NewPopulation.Num() * SelectionPercent);
	for(;NewPopulation.Num() > SelectedPopulationCount;)
	{
		NewPopulation.Pop(false);
	}
	Algo::RandomShuffle(NewPopulation);

	TArray<URoomTraits*> CrossoverPopulation = NewPopulation;
	const int32 CrossoverPopulationCount = FMath::RoundToInt32(SelectedPopulationCount * CrossoverPercent);
	for(;CrossoverPopulation.Num() > CrossoverPopulationCount;)
	{
		CrossoverPopulation.Pop(false);
	}
	for(int32 i = 0; i < CrossoverPopulation.Num() / 2; ++i)
	{
		TraitsPopulation.Add(CrossoverPopulation[i]->Crossover(CrossoverPopulation[CrossoverPopulation.Num() / 2 + i]));
	}
	if(CrossoverPopulation.Num() % 2 == 1)
	{
		TraitsPopulation.Add(CrossoverPopulation[0]->Crossover(CrossoverPopulation.Last()));
	}

	TArray<URoomTraits*> MutatePopulation = NewPopulation;
	const int32 MutatePopulationCount = SelectedPopulationCount - CrossoverPopulationCount;
	for(;MutatePopulation.Num() > MutatePopulationCount;)
	{
		MutatePopulation.RemoveAt(0);
	}
	for(URoomTraits* Traits : MutatePopulation)
	{
		TraitsPopulation.Remove(Traits);
		Traits->Mutate(MutateGrowChance, MutateCutChance, 1, 1);
		TraitsPopulation.Add(Traits);
	}

	GetSortedPopulation(NewPopulation);
	for(int32 i = 0; i < BestCount; ++i)
	{
		OutTraits.Add(NewPopulation[i]);
		GeneticAlgorithmSave->AddRoomTraitsToSave(NewPopulation[i]);
	}
}

void UTraitGeneticAlgorithm::GetSortedPopulation(TArray<URoomTraits*>& OutPopulation)
{
	OutPopulation.Empty();
	OutPopulation = TraitsPopulation;
	OutPopulation.Sort([](const URoomTraits& Left, const URoomTraits& Right)//ascending order
	{
		return Left.GetFitnessValue() < Right.GetFitnessValue();
	});
}
