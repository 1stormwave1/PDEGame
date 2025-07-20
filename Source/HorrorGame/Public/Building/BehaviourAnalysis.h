

#pragma once

#include "CoreMinimal.h"
#include "BehaviourAnalysis.generated.h"

UENUM(BlueprintType)
enum class BehaviourType : uint8
{
	None = 0,
	Killer = 1,
	Achiever = 2,
	Socializer = 3,
	Explorer = 4
};

USTRUCT(BlueprintType, Blueprintable)
struct FBehaviourVector
{
	FBehaviourVector() {}
	
	FBehaviourVector(const FBehaviourVector& Other)
		: CellsOverlappedPercentage(Other.CellsOverlappedPercentage),
		  TreasuresCollectedPercentage(Other.TreasuresCollectedPercentage),
		  GlobalsCollectedPercentage(Other.GlobalsCollectedPercentage),
		  DialoguesPercentage(Other.DialoguesPercentage),
		  DialoguesSkippedPercentage(Other.DialoguesSkippedPercentage),
		  EnemiesKilledPercentage(Other.EnemiesKilledPercentage),
		  EnemiesAvoidedPercentage(Other.EnemiesAvoidedPercentage),
		  ItemsInteractedPercentage(Other.ItemsInteractedPercentage)
	{
	}

	FBehaviourVector(FBehaviourVector&& Other) noexcept
		: CellsOverlappedPercentage(Other.CellsOverlappedPercentage),
		  TreasuresCollectedPercentage(Other.TreasuresCollectedPercentage),
		  GlobalsCollectedPercentage(Other.GlobalsCollectedPercentage),
		  DialoguesPercentage(Other.DialoguesPercentage),
		  DialoguesSkippedPercentage(Other.DialoguesSkippedPercentage),
		  EnemiesKilledPercentage(Other.EnemiesKilledPercentage),
		  EnemiesAvoidedPercentage(Other.EnemiesAvoidedPercentage),
		  ItemsInteractedPercentage(Other.ItemsInteractedPercentage)
	{
	}

	FBehaviourVector& operator=(const FBehaviourVector& Other)
	{
		if (this == &Other)
			return *this;
		CellsOverlappedPercentage = Other.CellsOverlappedPercentage;
		TreasuresCollectedPercentage = Other.TreasuresCollectedPercentage;
		GlobalsCollectedPercentage = Other.GlobalsCollectedPercentage;
		DialoguesPercentage = Other.DialoguesPercentage;
		DialoguesSkippedPercentage = Other.DialoguesSkippedPercentage;
		EnemiesKilledPercentage = Other.EnemiesKilledPercentage;
		EnemiesAvoidedPercentage = Other.EnemiesAvoidedPercentage;
		ItemsInteractedPercentage = Other.ItemsInteractedPercentage;
		return *this;
	}

	FBehaviourVector& operator=(FBehaviourVector&& Other) noexcept
	{
		if (this == &Other)
			return *this;
		CellsOverlappedPercentage = Other.CellsOverlappedPercentage;
		TreasuresCollectedPercentage = Other.TreasuresCollectedPercentage;
		GlobalsCollectedPercentage = Other.GlobalsCollectedPercentage;
		DialoguesPercentage = Other.DialoguesPercentage;
		DialoguesSkippedPercentage = Other.DialoguesSkippedPercentage;
		EnemiesKilledPercentage = Other.EnemiesKilledPercentage;
		EnemiesAvoidedPercentage = Other.EnemiesAvoidedPercentage;
		ItemsInteractedPercentage = Other.ItemsInteractedPercentage;
		return *this;
	}

	friend bool operator==(const FBehaviourVector& Lhs, const FBehaviourVector& RHS)
	{
		return Lhs.CellsOverlappedPercentage == RHS.CellsOverlappedPercentage
			&& Lhs.TreasuresCollectedPercentage == RHS.TreasuresCollectedPercentage
			&& Lhs.GlobalsCollectedPercentage == RHS.GlobalsCollectedPercentage
			&& Lhs.DialoguesPercentage == RHS.DialoguesPercentage
			&& Lhs.DialoguesSkippedPercentage == RHS.DialoguesSkippedPercentage
			&& Lhs.EnemiesKilledPercentage == RHS.EnemiesKilledPercentage
			&& Lhs.EnemiesAvoidedPercentage == RHS.EnemiesAvoidedPercentage
			&& Lhs.ItemsInteractedPercentage == RHS.ItemsInteractedPercentage;
	}

	friend bool operator!=(const FBehaviourVector& Lhs, const FBehaviourVector& RHS)
	{
		return !(Lhs == RHS);
	}

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CellsOverlappedPercentage = -1.f; //explorer+

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TreasuresCollectedPercentage = -1.f; //explorer+, achiever+

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GlobalsCollectedPercentage = -1.f; //achiever+

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DialoguesPercentage = -1.f; //socializer+

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DialoguesSkippedPercentage = -1.f; //socializer-

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemiesKilledPercentage = -1.f; //killer+

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemiesAvoidedPercentage = -1.f; //killer-

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemsInteractedPercentage = -1.f; //explorer+
	
};


class UBehaviourSaveGame;

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UBehaviourAnalysis : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBehaviourSaveGame> BehaviourSaveGameClass;

	UPROPERTY(EditAnywhere)
	FString BehaviourSaveGameSlotName = "BehaviourSave";
	
	UPROPERTY(Transient)
	UBehaviourSaveGame* BehaviourSaveGame = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TArray<FBehaviourVector> BehaviourData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBehaviourVector CurrentBehaviour;

	UPROPERTY(EditAnywhere)
	int32 MinCountForClustering = 5;

	UPROPERTY(EditAnywhere)
	float FuzzinessParameter = 2.f;

	UPROPERTY(BlueprintReadWrite)
	TMap<BehaviourType, float> FuzzyClusteringResult =
	{
		TPair<BehaviourType, float>(BehaviourType::Killer, 0.25f),
		TPair<BehaviourType, float>(BehaviourType::Explorer, 0.25f),
		TPair<BehaviourType, float>(BehaviourType::Achiever, 0.25f),
		TPair<BehaviourType, float>(BehaviourType::Socializer, 0.25f)
	};

	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void SetCurrentCellsOverlappedPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentTreasuresPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentGlobalsPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentDialoguesPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentDialoguesSkippedPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentEnemiesKilledPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentEnemiesAvoidedPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintCallable)
	void SetCurrentItemsInteractedPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintPure)
	float GetPercentage(int32 AllCount, int32 PercentageCount);

	UFUNCTION(BlueprintCallable)
	void Execute();

	UFUNCTION(BlueprintCallable)
	void AddCurrentBehaviour();

private:
	void FuzzyCMeans(int32 ClustersCount = 4);

	void InitializeFCMMatrix(TArray<TArray<float>>& FCMMatrix, int32 ClusterCount);

	void CalculateCentroids(TArray<TArray<float>>& OutCentroids, const TArray<TArray<float>>& FCMMatrix);

	void GetRawBehaviourData(TArray<TArray<float>> OutRawBehaviourData);

	void CalculateDistancesToCentroids(TArray<TArray<float>>& OutDistances, const TArray<TArray<float>>& Centroids);

	void CalculateNewFCMMatrix(TArray<TArray<float>>& NewFCMMatrix, const TArray<TArray<float>>& DistancesToCentroids);

	float GetEuclideanDistance(const TArray<float>& Start, const TArray<float>& Finish);

	float GetTolerance(const TArray<TArray<float>> Old, const TArray<TArray<float>> New);
	
};
