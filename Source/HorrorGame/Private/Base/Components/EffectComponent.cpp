


#include "Base/Components/EffectComponent.h"

#include "Base/HorrorGameCharacter.h"
#include "Effects/Effect.h"


void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	HorrorGameCharacter = GetOwner<AHorrorGameCharacter>();
}

void UEffectComponent::OnEffectRemoved(UEffect* Effect)
{
	CurrentEffects.Remove(Effect);
	OnEffectRemovedDelegate.Broadcast(Effect);
}

void UEffectComponent::RemoveImmunityEffect_Implementation(TSubclassOf<UEffect> EffectClass)
{
	ImmunityEffects.Remove(EffectClass);
	OnImmunityEffectRemovedDelegate.Broadcast(EffectClass->GetDefaultObject<UEffect>());
}

void UEffectComponent::AddImmunityEffect_Implementation(TSubclassOf<UEffect> EffectClass)
{
	ImmunityEffects.Add(EffectClass);
	OnImmunityEffectAddedDelegate.Broadcast(EffectClass->GetDefaultObject<UEffect>());
}

void UEffectComponent::RemoveEffectByClass_Implementation(TSubclassOf<UEffect> EffectClass)
{
	CurrentEffects.RemoveAll([this, EffectClass](UEffect* Effect)
	{
		if(Effect != nullptr && Effect->IsA(EffectClass))
		{
			Effect->RemoveEffect();
			OnEffectRemovedDelegate.Broadcast(Effect);
		}
		return Effect == nullptr || Effect->IsA(EffectClass);
	});
}

void UEffectComponent::ApplyEffectByClass_Implementation(TSubclassOf<UEffect> EffectClass)
{
	if(ImmunityEffects.Contains(EffectClass))
	{
		OnImmunityEffectTriggeredDelegate.Broadcast(EffectClass->GetDefaultObject<UEffect>());
		return;
	}
	
	for(UEffect* Effect : CurrentEffects)
	{
		if(Effect != nullptr && Effect->GetClass() == EffectClass)
		{
			Effect->ApplyEffect();
			OnEffectReappliedDelegate.Broadcast(Effect);
			return;
		}
	}
	
	UEffect* NewEffect = NewObject<UEffect>(this, EffectClass);
	NewEffect->SetEffectComponent(this);
	NewEffect->ApplyEffect();
	CurrentEffects.Add(NewEffect);
	OnEffectAppliedDelegate.Broadcast(NewEffect);
	NewEffect->OnEffectRemoved.AddDynamic(this, &UEffectComponent::OnEffectRemoved);
}

