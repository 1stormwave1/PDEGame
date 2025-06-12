

#pragma once

#include "CoreMinimal.h"
#include "Building/BSPTree.h"
#include "Engine/DataAsset.h"
#include "PDABSPTree.generated.h"


class UTraitGeneticAlgorithm;

UCLASS()
class HORRORGAME_API UPDABSPTree : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBSPTree> TreeClass;

	UPROPERTY(BlueprintReadWrite, Transient)
	UBSPTree* Tree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UTraitGeneticAlgorithm>> TTGeneticAlgorithmClasses;

	UPROPERTY(BlueprintReadWrite, Transient)
	TArray<UTraitGeneticAlgorithm*> TTGeneticAlgorithms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	TArray<FBSPNode> RoomsData;
	
	UFUNCTION(BlueprintCallable)
	void RetrieveRooms();

	UFUNCTION(BlueprintCallable)
	void InitializeTree();

	UFUNCTION(BlueprintCallable)
	void InitializeRandomTree(int32 RoomsCount, int32 AvailableZonesCount);

	UFUNCTION(BlueprintCallable)
	void InitializeGeneticAlgorithm();

	UFUNCTION(BlueprintCallable)
	void ExecuteGeneticAlgorithm();
	
};
