

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "HorrorGameController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryToggledSignature);

class UInputMappingContext;

UCLASS()
class HORRORGAME_API AHorrorGameController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* InventoryInteractionContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* ScrollInputAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* UseItemInputAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* InventoryInputAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* PickUpInputAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* PauseAction;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<class UInventoryMainSlotInput>> InventoryMainSlotInputs;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventoryComponent;
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryToggledSignature OnInventoryToggle;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<class UInteractionComponent*> InteractionQueue;
	
	AHorrorGameController();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupItemActor(int32 NewCurrentActiveSlotIndex);

	UFUNCTION()
	void ScrollActiveSlot(const FInputActionValue& Value);

	UFUNCTION()
	void OnItemStartUse();

	UFUNCTION()
	void OnItemUsing();

	UFUNCTION()
	void OnItemFinishUse();

	UFUNCTION()
	void PauseGame();

	UFUNCTION(BlueprintNativeEvent)
	void ToggleInventory();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CancelInteract();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
};