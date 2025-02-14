


#include "Building/PCGSettings.h"

UWorld* UPCGSettings::GetWorld() const
{
	return ActorOwner != nullptr ? ActorOwner->GetWorld() : UObject::GetWorld();
}

void UPCGSettings::Initialize(AActor* Owner)
{
	ActorOwner = Owner;
}
