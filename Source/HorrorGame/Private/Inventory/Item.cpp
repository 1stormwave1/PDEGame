


#include "Inventory/Item.h"
#include "Base/HorrorGameState.h"
#include "Base/HorrorPlayerState.h"
#include "Inventory/FillUpItem.h"

UWorld* UItem::GetWorld() const
{
	return GetOuter()->GetWorld();
}

void UItem::StartUsePower()
{
	if(bIsLimitedPower && !bIsBurnOutPower)
	{
		GetWorld()->GetTimerManager().SetTimer(
			PowerTimerHandle, this, &UItem::TickPowerTimer, TickTime, true);
	}
}

void UItem::StopUsePower()
{
	if(bIsLimitedPower)
	{
		GetWorld()->GetTimerManager().PauseTimer(PowerTimerHandle);
	}
}

void UItem::BurnOutPower()
{
	bIsBurnOutPower = true;
	OnBurnOutPowerChanged.Broadcast(bIsBurnOutPower);
}

void UItem::FillUpPower(const TArray<UFillUpItem*>& Items)
{
	if(!bIsLimitedPower)
	{
		return;
	}

	const bool bWasBurnOut = CurrentPower <= 0.f;

	for(int32 i = 0; i < Items.Num(); ++i)
	{
		if(FillUpItems.Contains(Items[i]->GetClass()))
		{
			CurrentPower += Items[i]->FillValue;
			CurrentPower = FMath::Clamp(CurrentPower, 0.f, MaxPower);
		}
	}
	OnPowerChanged.Broadcast(CurrentPower);

	if(bWasBurnOut)
	{
		bIsBurnOutPower = false;
		OnBurnOutPowerChanged.Broadcast(bIsBurnOutPower);
		
		bIsBurnOut = false;
		OnBurnOutChanged.Broadcast(bIsBurnOut);
	}
}

FText UItem::GetFormattedDescription()
{
	FFormatNamedArguments Args;
	TArray<FString> ArgsKeys;
	FormatArgs.GetKeys(ArgsKeys);
	for(FString& ArgName : ArgsKeys)
	{
		Args.Add(ArgName, FormatArgs[ArgName]);
	}
	return FText::Format(ItemDescription, Args);
}

void UItem::PostInitProperties()
{
	UObject::PostInitProperties();

	SetFormatArgs();
}

AHorrorGameState* UItem::GetGameState() const
{
	return GetWorld() ? GetWorld()->GetGameState<AHorrorGameState>() : nullptr;
}

AHorrorPlayerState* UItem::GetPlayerState() const
{
	return GetWorld() ? GetWorld()->GetFirstPlayerController()->GetPlayerState<AHorrorPlayerState>() : nullptr;
}

void UItem::SetFormatArgs_Implementation()
{
}

void UItem::ActivateItem_Implementation()
{
}

void UItem::DisactivateItem_Implementation()
{
}

void UItem::TickPowerTimer_Implementation()
{
	CurrentPower -= TickTime / UseTime;
	CurrentPower = FMath::Clamp(CurrentPower, 0.f, MaxPower);
	OnPowerChanged.Broadcast(CurrentPower);

	if(CurrentPower <= 0.f)
	{
		bIsBurnOut = true;
		OnBurnOutChanged.Broadcast(bIsBurnOut);

		GetWorld()->GetTimerManager().ClearTimer(PowerTimerHandle);
	}
}
