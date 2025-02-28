


#include "Building\PCGBuildingSettings.h"

UWorld* UPCGBuildingSettings::GetWorld() const
{
	return ActorOwner != nullptr ? ActorOwner->GetWorld() : UObject::GetWorld();
}

void UPCGBuildingSettings::Initialize(AActor* Owner)
{
	ActorOwner = Owner;
}
