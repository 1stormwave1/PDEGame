


#include "Building/TraitGeneticAlgorithm.h"

#include "Building/GeneticAlgorithmSave.h"

void UTraitGeneticAlgorithm::InitializeByRoomType(ERoomTypeEnum NewRoomType)
{
	Type = NewRoomType;
}

void UTraitGeneticAlgorithm::Initialize(UGeneticAlgorithmSave* NewGeneticAlgorithmSave, UBehaviourAnalysis* BehaviourAnalysis,int32 MinPopulationCount)
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

		if(BehaviourAnalysis != nullptr)
		{
			NewRoomTraits->FuzzyClusteringResult = BehaviourAnalysis->FuzzyClusteringResult;
		}
		
		TraitsPopulation.Add(NewRoomTraits);
	}

	if(TraitsPopulation.Num() < MinPopulationCount)
	{
		const int32 AdditionalTraitsCount = MinPopulationCount - TraitsPopulation.Num();
		for(int32 i = 0; i < AdditionalTraitsCount; ++i)
		{
			URoomTraits* NewRoomTraits = NewObject<URoomTraits>(this, RoomTraitsClass);
			NewRoomTraits->InitializeDefault();

			if(BehaviourAnalysis != nullptr)
			{
				NewRoomTraits->FuzzyClusteringResult = BehaviourAnalysis->FuzzyClusteringResult;
			}

			TraitsPopulation.Add(NewRoomTraits);
		}
	}
	else
	{
		//+1 залежно від типу гравця (після аналізу даних)
		URoomTraits* NewRoomTraits = NewObject<URoomTraits>(this, RoomTraitsClass);
		NewRoomTraits->InitializeDefault();

		if(BehaviourAnalysis != nullptr)
		{
			NewRoomTraits->FuzzyClusteringResult = BehaviourAnalysis->FuzzyClusteringResult;
		}

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
	RouletteWheelSelection(NewPopulation);
	
	TArray<URoomTraits*> CrossoverPopulation = NewPopulation;
	const int32 CrossoverPopulationCount = FMath::RoundToInt32(NewPopulation.Num() * CrossoverPercent);
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
	const int32 MutatePopulationCount = NewPopulation.Num() - CrossoverPopulationCount;
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

	GetSortedPopulationByFitnessValue(NewPopulation);
	
	for(int32 i = 0; i < BestCount; ++i)
	{
		OutTraits.Add(NewPopulation[i]);
		GeneticAlgorithmSave->AddRoomTraitsToSave(NewPopulation[i]);
	}
}

void UTraitGeneticAlgorithm::GetSortedPopulationByFitnessValue(TArray<URoomTraits*>& OutPopulation)
{
	OutPopulation.Empty();
	OutPopulation = TraitsPopulation;
	OutPopulation.Sort([](const URoomTraits& Left, const URoomTraits& Right)//descending order
	{
		return Left.GetFitnessValue() > Right.GetFitnessValue();
	});
}

void UTraitGeneticAlgorithm::RouletteWheelSelection(TArray<URoomTraits*>& OutPopulation)
{
	TArray<float> Probabilities;
	float FitnessSum = 0.f;
	
	for(const URoomTraits* RoomTraits : TraitsPopulation)
	{
		FitnessSum += RoomTraits->GetFitnessValue();
	}

	float CumulativeProbability = 0.f;
	for(const URoomTraits* RoomTraits : TraitsPopulation)
	{
		Probabilities.Add(CumulativeProbability);
		CumulativeProbability += RoomTraits->GetFitnessValue() / FitnessSum;
	}
	Probabilities.Add(CumulativeProbability);

	for(int32 i = 0; i < TraitsPopulation.Num(); ++i)
	{
		const float RandomValue = FMath::RandRange(0.f, 1.f);
		for(int32 j = 0; j < Probabilities.Num() - 1; ++j)
		{
			if(RandomValue >= Probabilities[j] && RandomValue < Probabilities[j + 1])
			{
				OutPopulation.Add(TraitsPopulation[j]->GetCopy(this));
			}
		}
	}
}
