
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Base/Components/DialogueComponent.h"
#include "HorrorHUD.generated.h"

class UUserWidget;

UENUM(BlueprintType)
enum class EDisplayName : uint8
{
	None = 0,
	Main = 1,
	Pause = 2,
	Interaction = 3,
	Dialogue = 4
};

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	None = 0,
	UIOnly = 1,
	GameAndUI = 2,
	GameOnly = 3
};

USTRUCT()
struct FHUDWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere)
	bool bShouldDestroyOnSwitch = true;
	
	UPROPERTY(EditAnywhere)
	int32 ZOrder = 0;

	UPROPERTY(EditAnywhere)
	EInputMode InputMode;

	UPROPERTY(EditAnywhere)
	EMouseLockMode MouseLockMode;

	UPROPERTY(EditAnywhere)
	bool bHideCursorDuringCapture = true;

	UPROPERTY(EditAnywhere)
	bool bShowMouseCursor = false;

	UPROPERTY(EditAnywhere)
	bool bIsRemovedWhenChanged = true;
};

UCLASS()
class HORRORGAME_API AHorrorHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EDisplayName, FHUDWidgetData> HUDWidgets;

	UPROPERTY(Transient)
	TArray<UUserWidget*> CurrentWidgetCache;

	UPROPERTY(EditAnywhere)
	int32 MaxCacheSize = 3;

	UPROPERTY(EditAnywhere)
	EDisplayName StartDisplay;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UUserWidget> CurrentWidget = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	EDisplayName CurrentDisplayName;

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetDisplay(EDisplayName NewDisplayName);

	FHUDWidgetData* GetHighestWidgetData();

	FHUDWidgetData* GetWidgetData(EDisplayName DisplayName);

	FHUDWidgetData* GetWidgetData(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void SetInputMode(EInputMode InputMode, EMouseLockMode MouseLockMode,
		bool bHideCursorDuringCapture, bool bShowMouseCursor);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDialogueStartedSignature OnDialogueStarted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDialogueSkipSignature OnDialogueSkip;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDialogueEndedSignature OnDialogueEnded;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDialogueNextStepSignature OnDialogueNextStep;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDialogueResponseReceivedSignature OnDialogueResponseReceived;
};
