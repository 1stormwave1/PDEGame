


#include "DataAssets/PDABSPTree.h"

#include "Building/BehaviourAnalysis.h"
#include "Building/GeneticAlgorithmSave.h"
#include "Building/TraitGeneticAlgorithm.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"

void UPDABSPTree::RetrieveRooms()
{
	if(BSPTree != nullptr)
	{
		BSPTree->GetLeaves(RoomsData);
	}
}

void UPDABSPTree::InitializeTree()
{
	if(BSPTree == nullptr)
	{
		BSPTree = NewObject<UBSPTree>(this, TreeClass);
		RoomsData.Empty();
	}
}

void UPDABSPTree::InitializeRandomTree(int32 RoomsCount, int32 AvailableZonesCount)
{
	if(BSPTree == nullptr)
	{
		BSPTree = NewObject<UBSPTree>(this, TreeClass);
		RoomsData.Empty();
	}
	BSPTree->InitializeRandom(RoomsCount, AvailableZonesCount);
}

void UPDABSPTree::InitializeGeneticAlgorithm()
{
	TGeneticAlgorithms.Empty();
	
	const int32 RoomsCount = RoomsData.Num();

	if(RoomsCount < TGeneticAlgorithmClasses.Num())
	{
		return;
	}

	if(USaveGame* SaveObject = UGameplayStatics::LoadGameFromSlot(GeneticAlgorithmSaveSlotName, 0))
	{
		GeneticAlgorithmSave = Cast<UGeneticAlgorithmSave>(SaveObject);
	}

	if(GeneticAlgorithmSave == nullptr)
	{
		GeneticAlgorithmSave =
			Cast<UGeneticAlgorithmSave>(UGameplayStatics::CreateSaveGameObject(GeneticAlgorithmSaveClass));
	}
	
	for(TSubclassOf<UTraitGeneticAlgorithm> GA : TGeneticAlgorithmClasses)
	{
		UTraitGeneticAlgorithm* NewGA = NewObject<UTraitGeneticAlgorithm>(this, GA);

		switch(NewGA->GetRoomType())
		{
		case ERoomTypeEnum::Transition:
			NewGA->Initialize(GeneticAlgorithmSave, BehaviourAnalysis, RoomsCount - 2);
			break;
		default:
			NewGA->Initialize(GeneticAlgorithmSave, BehaviourAnalysis);
			break;
		}
		
		TGeneticAlgorithms.Add(NewGA);
	}
}

void UPDABSPTree::InitializeRoomTypes()
{
	if(RoomsData.IsEmpty() || BSPTree == nullptr)
	{
		return;
	}

	const bool bIsStartFromHead = FMath::RandBool();
	const int32 IndexVariation = FMath::RoundToInt32(
		RoomsData.Num() * FMath::Clamp(BSPTree->StartToFinishVariation, 0.f, 1.f));

	int32 StartIndex = -1;
	int32 FinishIndex = -1;
	
	if(bIsStartFromHead)
	{
		StartIndex = FMath::RandRange(0, IndexVariation);
		FinishIndex = FMath::RandRange(RoomsData.Num() - IndexVariation - 1, RoomsData.Num() - 1);
	}
	else
	{
		FinishIndex = FMath::RandRange(0, IndexVariation);
		StartIndex = FMath::RandRange(RoomsData.Num() - IndexVariation, RoomsData.Num() - 1);
	}

	RoomsData[StartIndex].RoomTraits = StartRoomTraitsClass != nullptr ? NewObject<URoomTraits>(this, StartRoomTraitsClass) : nullptr;
	RoomsData[FinishIndex].RoomTraits = FinishRoomTraitsClass != nullptr ? NewObject<URoomTraits>(this, FinishRoomTraitsClass) : nullptr;


	if(TransitionRoomTraitsClass == nullptr)
	{
		return;
	}
	for(FBSPNode& Node : RoomsData)
	{
		if(Node.RoomTraits == nullptr)
		{
			Node.RoomTraits = NewObject<URoomTraits>(this, TransitionRoomTraitsClass);
		}
	}
}

void UPDABSPTree::ExecuteGeneticAlgorithm()
{
	const int32 RoomsCount = RoomsData.Num();

	UTraitGeneticAlgorithm* StartGA = nullptr;
	UTraitGeneticAlgorithm* FinishGA = nullptr;

	for(UTraitGeneticAlgorithm* GA : TGeneticAlgorithms)
	{
		switch(GA->GetRoomType())
		{
		case ERoomTypeEnum::Start:
			StartGA = GA;
			break;
		case ERoomTypeEnum::Finish:
			FinishGA = GA;
			break;
		default:
			break;
		}
	}

	if(StartGA == nullptr || FinishGA == nullptr)
	{
		return;
	}
	
	TArray<URoomTraits*> OutTraits;
	
	StartGA->Execute(OutTraits, 1);
	FinishGA->Execute(OutTraits, 1);

	const int32 RoomsPerTransitionGA = (RoomsCount - 2) / (TGeneticAlgorithms.Num() - 2);
	
	for(UTraitGeneticAlgorithm* GA : TGeneticAlgorithms)
	{
		if(GA != StartGA && GA != FinishGA)
		{
			GA->Execute(OutTraits, RoomsPerTransitionGA);
		}
	}
	UGameplayStatics::SaveGameToSlot(GeneticAlgorithmSave, GeneticAlgorithmSaveSlotName, 0);
	
	for(int32 i = 0, j = 2; i < RoomsData.Num(); ++i)
	{
		switch(RoomsData[i].RoomTraits->RoomType)
		{
		case ERoomTypeEnum::Start:
			RoomsData[i].RoomTraits = OutTraits[0];
			break;
		case ERoomTypeEnum::Finish:
			RoomsData[i].RoomTraits = OutTraits[1];
			break;
		default:
			RoomsData[i].RoomTraits = OutTraits[j];
			if(++j > OutTraits.Num() - 1)
			{
				j = 2;
			}
			break;
		}
		
	}
}

void UPDABSPTree::InitializeBehaviourAnalysis()
{
	if(BehaviourAnalysisClass == nullptr)
	{
		return;
	}
	
	BehaviourAnalysis = NewObject<UBehaviourAnalysis>(this, BehaviourAnalysisClass);
	BehaviourAnalysis->Initialize();
}

void UPDABSPTree::ExecuteBehaviourAnalysis()
{
	if(BehaviourAnalysis == nullptr)
	{
		return;
	}
	BehaviourAnalysis->Execute();
}
