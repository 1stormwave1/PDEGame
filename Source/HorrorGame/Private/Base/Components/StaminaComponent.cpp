#include "HorrorGame/Public/Base/Components/StaminaComponent.h"



void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	StartIncrease();
}

void UStaminaComponent::ChangeStaminaTick()
{
	if(bUseStamina)
	{
		const float OldCurrentStamina = CurrentStamina;
		CurrentStamina += CurrentTickDelta;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);

		if(OldCurrentStamina != CurrentStamina)
		{
			CurrentStaminaChanged.Broadcast(CurrentStamina);
		}
	}
}

void UStaminaComponent::StartIncrease()
{
	if(IncreaseTickHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(IncreaseTickHandle);
	}
	CurrentTickDelta = IncreasePerTick;
	GetWorld()->GetTimerManager().SetTimer(
		IncreaseTickHandle,
		this,
		&UStaminaComponent::ChangeStaminaTick, 
		IncreaseTickTime,
		true);
}

void UStaminaComponent::StartUseStamina()
{
	if(RestTimerTickHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(RestTimerTickHandle);
	}
	if(IncreaseTickHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(IncreaseTickHandle);
	}
	CurrentTickDelta = -DecreasePerTick;
	GetWorld()->GetTimerManager().SetTimer(
		DecreaseTickHandle,
		this,
		&UStaminaComponent::ChangeStaminaTick, 
		DecreaseTickTime,
		true);
	bIsUsingStamina = true;
}

void UStaminaComponent::StopUseStamina()
{
	if(RestTimerTickHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(RestTimerTickHandle);
	}
	if(DecreaseTickHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DecreaseTickHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(
		RestTimerTickHandle,
		this,
		&UStaminaComponent::StartIncrease, 
		RestTime,
		false);
	bIsUsingStamina = false;
}

void UStaminaComponent::SetMaxStamina(const float NewMaxStamina)
{
	MaxStamina = NewMaxStamina;
	MaxStaminaChangedSignature.Broadcast(MaxStamina);
}
