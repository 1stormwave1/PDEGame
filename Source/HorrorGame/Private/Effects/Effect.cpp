


#include "Effects/Effect.h"
#include "Base/HorrorGameState.h"
#include "Base/Components/EffectComponent.h"

void UEffect::ApplyEffect_Implementation()
{
	if(DurationHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	}
	if(!bIsInfinite)
	{
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, this, &UEffect::RemoveEffect, Duration);
	}
	if(bIsStackable && bIsApplied)
	{
	  	CurrentStack = FMath::Clamp(++CurrentStack, 0, StacksValues.Num() - 1);
		OnStackChanged.Broadcast(CurrentStack);
	}

	bIsApplied = true;
	OnEffectApplied.Broadcast(this);
}

void UEffect::RemoveEffect_Implementation()
{
	OnEffectRemoved.Broadcast(this);
}

float UEffect::GetStackValue_Implementation() const
{
	return StacksValues[FMath::Clamp(CurrentStack, 0, StacksValues.Num() - 1)];
}

void UEffect::ChangeStacksAmount_Implementation(int32 StacksDelta)
{
	if(bIsStackable)
	{
		CurrentStack += StacksDelta;
		CurrentStack = FMath::Clamp(CurrentStack, 0, StacksValues.Num() - 1);
		
		OnStackChanged.Broadcast(CurrentStack);
	}
}

AHorrorGameState* UEffect::GetGameState() const
{
	return GetWorld() ? GetWorld()->GetGameState<AHorrorGameState>() : nullptr;
}

UWorld* UEffect::GetWorld() const
{
	return EffectComponent != nullptr ? EffectComponent->GetWorld() : UObject::GetWorld();
}

void UEffect::ClearAndInvalidateTimer(const FTimerHandle& TimerHandle)
{
	if(GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().ClearTimer(const_cast<FTimerHandle&>(TimerHandle));
	}
}

AActor* UEffect::SpawnActorFromClass(TSubclassOf<AActor> ActorClass)
{
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = EffectComponent->GetOwner();
	return GetWorld()->SpawnActor(ActorClass, nullptr,  nullptr, ActorSpawnParameters);
}
