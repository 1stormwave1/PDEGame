


#include "Building/BuildingGameState.h"
#include "Building/PCGRoom.h"
#include "Building/PCGAlgorithm.h"
#include "Building/PCGBuildingNode.h"
#include "Building/PCGTree.h"

void ABuildingGameState::BeginPlay()
{
	Super::BeginPlay();

	if(PCGAlgorithmClass != nullptr)
	{
		PCGAlgorithm = NewObject<UPCGAlgorithm>(this, PCGAlgorithmClass);
		PCGAlgorithm->Initialize(this);

		SpawnRooms(PCGAlgorithm->MainTree->Root);
	}
}

void ABuildingGameState::SpawnRooms(UPCGBuildingNode* Root)
{
	if(Root == nullptr)
	{
		return;
	}
	
	FActorSpawnParameters Params;
	Params.Instigator = GetWorld()->GetFirstPlayerController()->GetPawn();
	Params.Owner = this;
	
	//spawn room
	
	if(!Root->IsLeaf())
	{
		for(UPCGBuildingNode* Node : Root->ChildNodes)
		{
			SpawnRooms(Node);
		}
	}
}

void ABuildingGameState::OnCurrentRoomChanged_Implementation(APCGRoom* NewRoom)
{
	if(NewRoom != CurrentRoom)
	{
		CurrentRoom = NewRoom;
		OnCurrentRoomChangedDelegate.Broadcast(CurrentRoom);
	}
}
