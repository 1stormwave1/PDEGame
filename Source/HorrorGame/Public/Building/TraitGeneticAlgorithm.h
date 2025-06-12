

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.h"
#include "TraitGeneticAlgorithm.generated.h"


UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UTraitGeneticAlgorithm : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ETraitEnum Trait = ETraitEnum::None;

	UPROPERTY(Transient)
	TArray<URoomTraitTree*> TTreesPopulation;
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeByTrait(ETraitEnum NewTrait);

	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void Execute(TArray<URoomTraitTree*>& OutTTrees, int32 BestCount = 1);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETraitEnum GetTrait() const { return Trait; }
	
};
