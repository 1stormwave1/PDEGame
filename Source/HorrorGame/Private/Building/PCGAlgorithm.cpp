


#include "Building/PCGAlgorithm.h"
#include "Building/PCGSettings.h"
#include "Building/PCGTree.h"

UWorld* UPCGAlgorithm::GetWorld() const
{
	return ActorOwner != nullptr ? ActorOwner->GetWorld() : UObject::GetWorld();
}

void UPCGAlgorithm::Initialize(AActor* Owner)
{
	ActorOwner = Owner;
	Settings = NewObject<UPCGSettings>(this, SettingsClass);
	Settings->Initialize(ActorOwner);
	Run();
}

void UPCGAlgorithm::Run_Implementation()
{
	MainTree = NewObject<UPCGTree>(this, TreeClass);
	MainTree->Initialize();
}
