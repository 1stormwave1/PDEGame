

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.h"
#include "TraitGeneticAlgorithm.generated.h"


UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UTraitGeneticAlgorithm : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERoomTypeEnum Type = ERoomTypeEnum::None;

	UPROPERTY(Transient)
	TArray<URoomTraits*> TraitsPopulation;
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeByRoomType(ERoomTypeEnum NewRoomType);

	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void Execute(TArray<URoomTraits*>& OutTraits, int32 BestCount = 1);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ERoomTypeEnum GetRoomType() const { return Type; }
	
};
