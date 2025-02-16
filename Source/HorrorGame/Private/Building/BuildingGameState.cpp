


#include "Building/BuildingGameState.h"
#include "Building/Room.h"
#include "Building/PCGAlgorithm.h"
#include "Building/PCGNode.h"
#include "Building/PCGTree.h"

void ABuildingGameState::BeginPlay()
{
	Super::BeginPlay();

	PCGAlgorithm = NewObject<UPCGAlgorithm>(this, PCGAlgorithmClass);
	PCGAlgorithm->Initialize(this);

	SpawnRooms(PCGAlgorithm->MainTree->Root);
}

void ABuildingGameState::SpawnRooms(UPCGNode* Root)
{
	if(Root == nullptr)
	{
		return;
	}
	
	FActorSpawnParameters Params;
	Params.Instigator = GetWorld()->GetFirstPlayerController()->GetPawn();
	Params.Owner = this;
	
	GetWorld()->SpawnActor<ARoom>(Root->RoomClass, Params);

	if(!Root->IsLeaf())
	{
		for(UPCGNode* Node : Root->ChildNodes)
		{
			SpawnRooms(Node);
		}
	}
}

void ABuildingGameState::OnCurrentRoomChanged_Implementation(ARoom* NewRoom)
{
	if(NewRoom != CurrentRoom)
	{
		CurrentRoom = NewRoom;
		OnCurrentRoomChangedDelegate.Broadcast(CurrentRoom);
	}
}
