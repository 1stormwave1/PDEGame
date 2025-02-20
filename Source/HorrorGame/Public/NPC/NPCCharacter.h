

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

class UInteractionComponent;
class UWidgetComponent;

UCLASS()
class HORRORGAME_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteractChanged(bool bIsInteractable);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteractCancel();
};
