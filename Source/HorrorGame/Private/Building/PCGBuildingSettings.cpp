


#include "..\..\Public\Building\PCGBuildingSettings.h"

UWorld* UPCGSettings::GetWorld() const
{
	return ActorOwner != nullptr ? ActorOwner->GetWorld() : UObject::GetWorld();
}

void UPCGSettings::Initialize(AActor* Owner)
{
	ActorOwner = Owner;
}
