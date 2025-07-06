

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.h"
#include "TraitGeneticAlgorithm.generated.h"


class UGeneticAlgorithmSave;

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UTraitGeneticAlgorithm : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERoomTypeEnum Type = ERoomTypeEnum::None;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URoomTraits> RoomTraitsClass;

	UPROPERTY(Transient)
	TArray<URoomTraits*> TraitsPopulation;

	UPROPERTY(Transient)
	UGeneticAlgorithmSave* GeneticAlgorithmSave;
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeByRoomType(ERoomTypeEnum NewRoomType);

	UFUNCTION(BlueprintCallable)
	void Initialize(UGeneticAlgorithmSave* NewGeneticAlgorithmSave, int32 MinPopulationCount = 1);

	UFUNCTION(BlueprintCallable)
	void Execute(TArray<URoomTraits*>& OutTraits, int32 BestCount = 1);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ERoomTypeEnum GetRoomType() const { return Type; }
	
};
