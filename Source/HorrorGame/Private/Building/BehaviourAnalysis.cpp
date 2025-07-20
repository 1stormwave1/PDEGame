


#include "Building/BehaviourAnalysis.h"

#include "Algo/MaxElement.h"
#include "Building/BehaviourSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UnrealTypePrivate.h"

void UBehaviourAnalysis::Initialize()
{
	if(USaveGame* SaveObject = UGameplayStatics::LoadGameFromSlot(BehaviourSaveGameSlotName, 0))
	{
		BehaviourSaveGame = Cast<UBehaviourSaveGame>(SaveObject);
	}

	if(BehaviourSaveGame == nullptr)
	{
		BehaviourSaveGame =
			Cast<UBehaviourSaveGame>(UGameplayStatics::CreateSaveGameObject(BehaviourSaveGameClass));
	}

	BehaviourData = BehaviourSaveGame->BehaviourData;
}

void UBehaviourAnalysis::SetCurrentCellsOverlappedPercentage(int32 AllCount, int32 PercentageCount)
{
	CurrentBehaviour.CellsOverlappedPercentage = GetPercentage(AllCount, PercentageCount);
}

void UBehaviourAnalysis::SetCurrentTreasuresPercentage(int32 AllCount, int32 PercentageCount)
{
	CurrentBehaviour.TreasuresCollectedPercentage = GetPercentage(AllCount, PercentageCount);
}

void UBehaviourAnalysis::SetCurrentGlobalsPercentage(int32 AllCount, int32 PercentageCount)
{
	CurrentBehaviour.GlobalsCollectedPercentage = GetPercentage(AllCount, PercentageCount);
}

void UBehaviourAnalysis::SetCurrentDialoguesPercentage(int32 AllCount, int32 PercentageCount)
{
	CurrentBehaviour.DialoguesPercentage = GetPercentage(AllCount, PercentageCount);
}

void UBehaviourAnalysis::SetCurrentDialoguesSkippedPercentage(int32 AllCount, int32 PercentageCount)
{
	CurrentBehaviour.DialoguesSkippedPercentage = GetPercentage(AllCount, PercentageCount);
}

void UBehaviourAnalysis::SetCurrentEnemiesKilledPercentage(int32 AllCount, int32 PercentageCount)
{
	CurrentBehaviour.EnemiesKilledPercentage = GetPercentage(AllCount, PercentageCount);
}

void UBehaviourAnalysis::SetCurrentEnemiesAvoidedPercentage(int32 AllCount, int32 PercentageCount)
{
	CurrentBehaviour.EnemiesAvoidedPercentage = GetPercentage(AllCount, PercentageCount);
}

void UBehaviourAnalysis::SetCurrentItemsInteractedPercentage(int32 AllCount, int32 PercentageCount)
{
	CurrentBehaviour.ItemsInteractedPercentage = GetPercentage(AllCount, PercentageCount);
}

float UBehaviourAnalysis::GetPercentage(int32 AllCount, int32 PercentageCount)
{
	if(AllCount != 0.f)
	{
		return static_cast<float>(PercentageCount) / AllCount;
	}
	return -1.f;
}

void UBehaviourAnalysis::Execute()
{
	if(BehaviourData.Num() < MinCountForClustering)
	{
		return;
	}
	
	FuzzyCMeans(4);
}

void UBehaviourAnalysis::AddCurrentBehaviour()
{
	BehaviourData.Add(CurrentBehaviour);
	BehaviourSaveGame->BehaviourData.Add(CurrentBehaviour);
	
	UGameplayStatics::SaveGameToSlot(BehaviourSaveGame, BehaviourSaveGameSlotName, 0);
}

void UBehaviourAnalysis::FuzzyCMeans(int32 ClustersCount)
{
	TArray<TArray<float>> FCMMatrix;
	TArray<TArray<float>> NewFCMMatrix;
	InitializeFCMMatrix(NewFCMMatrix, ClustersCount);

	do
	{
		FCMMatrix = NewFCMMatrix;
		TArray<TArray<float>> Centroids;
		TArray<TArray<float>> DistancesToCentroids;
		
		CalculateCentroids(Centroids, FCMMatrix);
		CalculateDistancesToCentroids(DistancesToCentroids, Centroids);
		CalculateNewFCMMatrix(NewFCMMatrix, DistancesToCentroids);
	}
	while (GetTolerance(FCMMatrix, NewFCMMatrix) > 0.01f);

	//
}

void UBehaviourAnalysis::InitializeFCMMatrix(TArray<TArray<float>>& FCMMatrix, int32 ClusterCount)
{
	FCMMatrix.SetNum(BehaviourData.Num());
	for(int32 i = 0; i < FCMMatrix.Num(); ++i)
	{
		FCMMatrix[i].SetNum(ClusterCount);
		float LeftMembership = 1.f;
		for(int32 j = 0; j < FCMMatrix[i].Num() - 1; ++j)
		{
			const float Membership = FMath::RandRange(0.f, LeftMembership);
			LeftMembership -= Membership;

			FCMMatrix[i][j] = Membership;
		}
		FCMMatrix[i][FCMMatrix[i].Num() - 1] = LeftMembership;
	}
}

void UBehaviourAnalysis::CalculateCentroids(TArray<TArray<float>>& OutCentroids,
	const TArray<TArray<float>>& FCMMatrix)
{
	TArray<TArray<float>> BehaviourDataRow;
	GetRawBehaviourData(BehaviourDataRow);
	
	for(int32 i = 0; i < FCMMatrix[0].Num(); ++i)
	{
		float MembershipSum = 0.f;
		for(int32 j = 0; j < FCMMatrix.Num(); ++j)
		{
			MembershipSum += FMath::Pow(FCMMatrix[j][i], FuzzinessParameter); 
		}

		TArray<float> RawCentroid;
		for(int32 j = 0; j < BehaviourDataRow[0].Num(); ++j)
		{
			float MembershipDataSum = 0.f;
			for(int32 k = 0; k < FCMMatrix.Num(); ++k)
			{
				MembershipDataSum += FMath::Pow(FCMMatrix[k][i], FuzzinessParameter) * BehaviourDataRow[k][j];
			}
			RawCentroid.Add(MembershipDataSum / MembershipSum);
		}

		OutCentroids.Add(RawCentroid);
		
	}
}

void UBehaviourAnalysis::GetRawBehaviourData(TArray<TArray<float>> OutRawBehaviourData)
{
	for(const FBehaviourVector& Vector : BehaviourData)
	{
		TArray<float> BehaviourVector;
		
		BehaviourVector.Add(Vector.CellsOverlappedPercentage);
		BehaviourVector.Add(Vector.TreasuresCollectedPercentage);
		BehaviourVector.Add(Vector.GlobalsCollectedPercentage);
		BehaviourVector.Add(Vector.DialoguesPercentage);
		BehaviourVector.Add(Vector.DialoguesSkippedPercentage);
		BehaviourVector.Add(Vector.EnemiesKilledPercentage);
		BehaviourVector.Add(Vector.EnemiesAvoidedPercentage);
		BehaviourVector.Add(Vector.ItemsInteractedPercentage);
		
		OutRawBehaviourData.Add(BehaviourVector);
	}
}

void UBehaviourAnalysis::CalculateDistancesToCentroids(TArray<TArray<float>>& OutDistances,
	const TArray<TArray<float>>& Centroids)
{
	TArray<TArray<float>> RawBehaviourData;
	GetRawBehaviourData(RawBehaviourData);

	for(int32 i = 0; i < RawBehaviourData.Num(); ++i)
	{
		TArray<float> Distance;
		for(int32 j = 0; j < Centroids.Num(); ++j)
		{
			Distance.Add(GetEuclideanDistance(RawBehaviourData[i], Centroids[j]));
		}
	}
}

void UBehaviourAnalysis::CalculateNewFCMMatrix(TArray<TArray<float>>& NewFCMMatrix,
	const TArray<TArray<float>>& DistancesToCentroids)
{
	NewFCMMatrix.Empty();
	for(int32 i = 0; i < DistancesToCentroids.Num(); ++i)
	{
		TArray<float> NewMembershipValues;
		for(int32 j = 0; j < DistancesToCentroids[i].Num(); ++j)
		{
			float MembershipValue = 0.f;

			for(int32 k = 0; k < DistancesToCentroids[i].Num(); ++k)
			{
				MembershipValue +=
					(DistancesToCentroids[i][j] * DistancesToCentroids[i][j]) /
						(DistancesToCentroids[i][k] * DistancesToCentroids[i][k]);
			}
			MembershipValue = FMath::Pow(MembershipValue, 1.f/(FuzzinessParameter - 1.f));
			MembershipValue = FMath::Pow(MembershipValue, -1.f);

			NewMembershipValues.Add(MembershipValue);
		}
		NewFCMMatrix.Add(NewMembershipValues);
	}
}

float UBehaviourAnalysis::GetEuclideanDistance(const TArray<float>& Start, const TArray<float>& Finish)
{
	float QuadDistance = 0.f;
	for(int32 i = 0; i < Start.Num() && Finish.Num(); ++i)
	{
		QuadDistance += (Finish[i] - Start[i]) *  (Finish[i] - Start[i]);
	}

	return FMath::Sqrt(QuadDistance);
}

float UBehaviourAnalysis::GetTolerance(const TArray<TArray<float>> Old, const TArray<TArray<float>> New)
{
	TArray<float> Eps;

	for(int32 i = 0; i < Old.Num() && New.Num(); ++i)
	{
		for(int32 j = 0; j < Old.Num() && New.Num(); ++j)
		{
			Eps.Add(FMath::Abs(Old[i][j] - New[i][j]));
		}
	}

	const float* MaxElement = Algo::MaxElement(Eps);

	if(MaxElement != nullptr)
	{
		return *MaxElement;
	}
	return 0.f;
}
