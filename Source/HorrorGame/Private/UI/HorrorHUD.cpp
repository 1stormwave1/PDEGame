


#include "UI/HorrorHUD.h"
#include "Blueprint/UserWidget.h"

void AHorrorHUD::BeginPlay()
{
	Super::BeginPlay();
	
	SetDisplay(StartDisplay);
}

void AHorrorHUD::SetDisplay(EDisplayName NewDisplayName)
{
	if(FHUDWidgetData* HUDWidgetData = GetWidgetData(CurrentDisplayName))
	{
		if(HUDWidgetData->bIsRemovedWhenChanged && CurrentWidget != nullptr)
		{
			CurrentWidget->RemoveFromParent();
		}
	}

	if(FHUDWidgetData* HUDWidgetData = GetWidgetData(NewDisplayName))
	{
		UUserWidget* NewDisplayWidget = nullptr;
		if(HUDWidgetData->bShouldDestroyOnSwitch)
		{
			NewDisplayWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetData->WidgetClass);
		}
		else
		{
			UUserWidget* FoundWidget = nullptr;
			for(UUserWidget* Widget : CurrentWidgetCache)
			{
				if(Widget->IsA(HUDWidgetData->WidgetClass))
				{
					FoundWidget = Widget;
					break;
				}
			}
			if(FoundWidget == nullptr)
			{
				FoundWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetData->WidgetClass);
				CurrentWidgetCache.Insert(FoundWidget, 0);
				if(CurrentWidgetCache.Num() > MaxCacheSize)
				{
					CurrentWidgetCache.RemoveAt(MaxCacheSize);
				}
			}
			NewDisplayWidget = FoundWidget;
		}
		CurrentWidget = NewDisplayWidget;
		CurrentDisplayName = NewDisplayName;
		NewDisplayWidget->AddToViewport(HUDWidgetData->ZOrder);
		const FHUDWidgetData* HighestWidgetData = GetHighestWidgetData();
		SetInputMode(HighestWidgetData->InputMode, HighestWidgetData->MouseLockMode,
			HighestWidgetData->bHideCursorDuringCapture, HighestWidgetData->bShowMouseCursor);
	}
}

FHUDWidgetData* AHorrorHUD::GetHighestWidgetData()
{
	UUserWidget* HighestWidget = CurrentWidget;
	FHUDWidgetData* HighestWidgetData = GetWidgetData(CurrentWidget);

	for(UUserWidget* Widget : CurrentWidgetCache)
	{
		if(HighestWidget == nullptr && Widget->GetVisibility() ==  ESlateVisibility::Visible)
		{
			HighestWidget = Widget;
			HighestWidgetData = GetWidgetData(Widget);
		}
		else
		{
			FHUDWidgetData* WidgetData = GetWidgetData(Widget);
			if(WidgetData != nullptr)
			{
				if(Widget->GetVisibility() == ESlateVisibility::Visible && WidgetData->ZOrder > HighestWidgetData->ZOrder)
				{
					HighestWidget = Widget;
					HighestWidgetData = WidgetData;
				}
			}
		}
	}

	return HighestWidgetData;
}

FHUDWidgetData* AHorrorHUD::GetWidgetData(EDisplayName DisplayName)
{
	return HUDWidgets.Find(DisplayName);
}

FHUDWidgetData* AHorrorHUD::GetWidgetData(UUserWidget* Widget)
{
	TArray<FHUDWidgetData> Values;
	HUDWidgets.GenerateValueArray(Values);
	for(FHUDWidgetData& WidgetData : Values)
	{
		if(Widget->IsA(WidgetData.WidgetClass))
		{
			return &WidgetData;
		}
	}
	return nullptr;
}

void AHorrorHUD::SetInputMode(EInputMode InputMode, EMouseLockMode MouseLockMode, bool bHideCursorDuringCapture,
	bool bShowMouseCursor)
{
	FInputModeDataBase* InputModeData = nullptr;
	switch (InputMode)
	{
	case EInputMode::UIOnly:
		{
			InputModeData = new FInputModeUIOnly();
			FInputModeUIOnly* UIOnlyInputModeData = dynamic_cast<FInputModeUIOnly*>(InputModeData);
			UIOnlyInputModeData->SetLockMouseToViewportBehavior(MouseLockMode);
		}
		break;
	case EInputMode::GameOnly:
		{
			InputModeData = new FInputModeGameOnly();
		}
		break;
	case EInputMode::GameAndUI:
	default:
		{
			InputModeData = new FInputModeGameAndUI();
			FInputModeGameAndUI* GameAndUIInputModeData = dynamic_cast<FInputModeGameAndUI*>(InputModeData);
			GameAndUIInputModeData->SetLockMouseToViewportBehavior(MouseLockMode);
			GameAndUIInputModeData->SetHideCursorDuringCapture(bHideCursorDuringCapture);
		}
	}
	GetOwningPlayerController()->SetInputMode(*InputModeData);
	GetOwningPlayerController()->SetShowMouseCursor(bShowMouseCursor);
}
