


#include "Building/TraitGeneticAlgorithm.h"

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
	}
}

void UTraitGeneticAlgorithm::Execute(TArray<URoomTraits*>& OutTraits, int32 BestCount)
{
	if(TraitsPopulation.Num() >=/*==*/ BestCount /*&& BestCount == 1*/)
	{
		for(int32 i = 0; i < BestCount; ++i)
		{
			OutTraits.Add(TraitsPopulation[i]);
			GeneticAlgorithmSave->AddRoomTraitsToSave(TraitsPopulation[i]);
		}
		return;
	}
}
