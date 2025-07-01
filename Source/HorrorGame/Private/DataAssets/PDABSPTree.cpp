


#include "DataAssets/PDABSPTree.h"
#include "Building/TraitGeneticAlgorithm.h"
#include "Kismet/KismetArrayLibrary.h"

void UPDABSPTree::RetrieveRooms()
{
	if(Tree != nullptr)
	{
		Tree->GetLeaves(RoomsData);
	}
}

void UPDABSPTree::InitializeTree()
{
	if(Tree == nullptr)
	{
		Tree = NewObject<UBSPTree>(this, TreeClass);
		RoomsData.Empty();
	}
}

void UPDABSPTree::InitializeRandomTree(int32 RoomsCount, int32 AvailableZonesCount)
{
	if(Tree == nullptr)
	{
		Tree = NewObject<UBSPTree>(this, TreeClass);
		RoomsData.Empty();
	}
	Tree->InitializeRandom(RoomsCount, AvailableZonesCount);
}

void UPDABSPTree::InitializeGeneticAlgorithm()
{
	const int32 RoomsCount = RoomsData.Num();

	if(RoomsCount < TTGeneticAlgorithmClasses.Num())
	{
		return;
	}

	for(TSubclassOf<UTraitGeneticAlgorithm> GA : TTGeneticAlgorithmClasses)
	{
		UTraitGeneticAlgorithm* NewGA = NewObject<UTraitGeneticAlgorithm>(this, GA);
		NewGA->Initialize();
		
		TTGeneticAlgorithms.Add(NewGA);
	}
}

void UPDABSPTree::ExecuteGeneticAlgorithm()
{
	const int32 RoomsCount = RoomsData.Num();

	UTraitGeneticAlgorithm* StartGA = nullptr;
	UTraitGeneticAlgorithm* FinishGA = nullptr;

	for(UTraitGeneticAlgorithm* GA : TTGeneticAlgorithms)
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

	const int32 RoomsPerTransitionGA = (RoomsCount - 2) / (TTGeneticAlgorithms.Num() - 2);
	
	for(UTraitGeneticAlgorithm* GA : TTGeneticAlgorithms)
	{
		if(GA != StartGA && GA != FinishGA)
		{
			GA->Execute(OutTraits, RoomsPerTransitionGA);
		}
	}

	//shuffle OutTraitTree based on where start and finish should be in a tree
	// [here]
	
	for(int32 i = 0; i < RoomsData.Num() && i < OutTraits.Num(); ++i)
	{
		RoomsData[i].RoomTraits = OutTraits[i];
	}
}
