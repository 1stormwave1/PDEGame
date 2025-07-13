

#pragma once

#include "CoreMinimal.h"
#include "Building/BSPTree.h"
#include "Engine/DataAsset.h"

#include "PDABSPTree.generated.h"


class UGeneticAlgorithmSave;
class UTraitGeneticAlgorithm;

UCLASS()
class HORRORGAME_API UPDABSPTree : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBSPTree> TreeClass;

	UPROPERTY(BlueprintReadWrite, Transient)
	UBSPTree* BSPTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Genetic Algorithm")
	TArray<TSubclassOf<UTraitGeneticAlgorithm>> TGeneticAlgorithmClasses;

	UPROPERTY(BlueprintReadWrite, Transient, Category="Genetic Algorithm")
	TArray<UTraitGeneticAlgorithm*> TGeneticAlgorithms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Genetic Algorithm")
	TSubclassOf<URoomTraits> StartRoomTraitsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Genetic Algorithm")
	TSubclassOf<URoomTraits> FinishRoomTraitsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Genetic Algorithm")
	TSubclassOf<URoomTraits> TransitionRoomTraitsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Genetic Algorithm")
	TSubclassOf<UGeneticAlgorithmSave> GeneticAlgorithmSaveClass;

	UPROPERTY(BlueprintReadWrite, Transient, Category="Genetic Algorithm")
	UGeneticAlgorithmSave* GeneticAlgorithmSave = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient, Category="Genetic Algorithm")
	FString GeneticAlgorithmSaveSlotName = "GeneticAlgorithmSave";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	TArray<FBSPNode> RoomsData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTraitsData> TraitsData;
	
	UFUNCTION(BlueprintCallable)
	void RetrieveRooms();

	UFUNCTION(BlueprintCallable)
	void InitializeTree();

	UFUNCTION(BlueprintCallable)
	void InitializeRandomTree(int32 RoomsCount, int32 AvailableZonesCount);

	UFUNCTION(BlueprintCallable)
	void InitializeGeneticAlgorithm();

	UFUNCTION(BlueprintCallable)
	void InitializeRoomTypes();

	UFUNCTION(BlueprintCallable)
	void ExecuteGeneticAlgorithm();
	
};
