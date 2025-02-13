


#include "Base/Components/HealthComponent.h"


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHealthComponent::ChangeCurrentHealth(float DeltaHealth)
{
	CurrentHealth += DeltaHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	OnCurrentHealthChanged.Broadcast(CurrentHealth);

	if(CurrentHealth <= 0.f)
	{
		Die();
	}
}

void UHealthComponent::Die_Implementation()
{
	OnDied.Broadcast();
}

