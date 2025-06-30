

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

class URotatingMovementComponent;
class UWidgetComponent;
class UInteractionComponent;
class UItem;

UCLASS()
class HORRORGAME_API AItemActor : public AActor
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> OffsetComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItem> DefaultItemClass;

public:
	AItemActor();
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItem> ItemOwner = nullptr;

	UPROPERTY(EditAnywhere)
	bool bInitializeItemOwner = false; //set true to auto create item owner
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartUseItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishUseItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnItemUsing();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetItemActorParams(UItem* Item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetVisibility(bool bNewVisibility);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteractabilityChanged(bool bIsInteractable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeItemOwner(bool bNewInitializeItemOwner);
	
};
