

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.h"
#include "TraitGeneticAlgorithm.generated.h"

class UBehaviourAnalysis;
class UGeneticAlgorithmSave;

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UTraitGeneticAlgorithm : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERoomTypeEnum Type = ERoomTypeEnum::None;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URoomTraits> RoomTraitsClass;

	UPROPERTY(EditAnywhere)
	float CrossoverPercent = 0.7f;

	UPROPERTY(EditAnywhere)
	float MutateGrowChance = 0.33f;

	UPROPERTY(EditAnywhere)
	float MutateCutChance = 0.33f;

	UPROPERTY(EditAnywhere)
	bool bIsExploration = true;

	UPROPERTY(EditAnywhere)
	float EntropyExploitationLevel = 1.5f;
	
	UPROPERTY(Transient)
	TArray<URoomTraits*> TraitsPopulation;

	UPROPERTY(Transient)
	UGeneticAlgorithmSave* GeneticAlgorithmSave;
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeByRoomType(ERoomTypeEnum NewRoomType);

	UFUNCTION(BlueprintCallable)
	void Initialize(UGeneticAlgorithmSave* NewGeneticAlgorithmSave, UBehaviourAnalysis* BehaviourAnalysis, int32 MinPopulationCount = 1);

	UFUNCTION(BlueprintCallable)
	void Execute(TArray<URoomTraits*>& OutTraits, int32 BestCount = 1);

	UFUNCTION(BlueprintCallable)
	void GetSortedPopulationByFitnessValue(TArray<URoomTraits*>& OutPopulation);

	UFUNCTION(BlueprintCallable)
	void RouletteWheelSelection(TArray<URoomTraits*>& OutPopulation);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ERoomTypeEnum GetRoomType() const { return Type; }

	UFUNCTION(Blueprintable)
	bool GetExplorationExploitationType(UBehaviourAnalysis* Behaviour);
	
};
