


#include "Building/RoomTraitTree.h"

#include "Algo/RandomShuffle.h"
#include "Kismet/KismetMathLibrary.h"

FTTNode::FTTNode(ETraitEnum NewTrait)
{
	Trait = NewTrait;
}

FTTNode::FTTNode()
{
}

void URoomTraits::Grow_Implementation()
{
	const int32 AddMutateTraitIndex = FMath::RandRange(0, TraitEnums.Num() - 1);

	FTTNode AddTrait(TraitEnums[AddMutateTraitIndex]);
	AddTrait.Weight = FMath::RandRange(0.f, 1.f);

	Traits.Add(AddTrait);
}

void URoomTraits::Cut_Implementation()
{
	if(Traits.Num() > 1)
	{
		const int32 RemoveIndex = FMath::RandRange(0, Traits.Num() - 1);
		Traits.RemoveAt(RemoveIndex);
	}
}

void URoomTraits::Alter_Implementation()
{
	if(Traits.Num() > 0)
	{
		const int32 ChangeIndex = FMath::RandRange(0, Traits.Num() - 1);
		const int32 NewTraitIndex = FMath::RandRange(0, TraitEnums.Num() - 1);

		Traits[ChangeIndex].Trait = TraitEnums[NewTraitIndex];
		Traits[ChangeIndex].Weight = FMath::RandRange(0.f, 1.f);
	}
}

void URoomTraits::Mutate_Implementation(float GrowChance, float CutChance, int32 MinIterationCount, int32 MaxIterationCount)
{
	const int32 IterationCount = FMath::RandRange(MinIterationCount, MaxIterationCount);

	for(int32 i = 0; i < IterationCount; ++i)
	{
		const float RandValue = FMath::RandRange(0.f, 1.f);

		if(RandValue >= 0.f && RandValue < GrowChance)
		{
			Grow();
		}
		else if(RandValue >= GrowChance && RandValue < GrowChance + CutChance)
		{
			Cut();
		}
		else
		{
			Alter();
		}
	}
}

URoomTraits* URoomTraits::Crossover_Implementation(URoomTraits* Other)
{
	URoomTraits* Child = NewObject<URoomTraits>(this, GetClass());
	
	const int32 TraitsCount = FMath::RoundToInt32((Traits.Num() + Other->Traits.Num()) / 2.f);

	TArray<FTTNode> MergedTraits;
	MergedTraits.Append(Traits);
	MergedTraits.Append(Other->Traits);
	Algo::RandomShuffle(MergedTraits);

	for(int32 i = 0; i < TraitsCount; ++i)
	{
		Child->Traits.Add(MergedTraits[i]);
	}
	Child->RoomType = RoomType;

	return Child;
}

float URoomTraits::GetFitnessValue() const
{
	float FitnessValue = 0.f;

	for(const FTTNode& Node : Traits)
	{
		FitnessValue += Node.Weight;
	}

	FitnessValue /= Traits.Num();

	return FitnessValue;
}

URoomTraits* URoomTraits::GetCopy(UObject* Owner) const
{
	URoomTraits* Copy = NewObject<URoomTraits>(Owner, GetClass());

	Copy->Traits = Traits;
	Copy->RoomType = RoomType;

	return Copy;
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
