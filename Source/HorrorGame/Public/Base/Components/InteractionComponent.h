
#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"


class UItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractChangedSignature, bool, bIsInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractedSignature);

class AHorrorGameController;

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(Transient, BlueprintReadWrite)
	AHorrorGameController* MainController = nullptr;

	bool bIsOverlapping = false;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent)
	void CheckOverlap(AActor* OtherActor);

public:
	UInteractionComponent();

	UPROPERTY(Transient)
	UItem* ItemOwner = nullptr;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CancelInteract();

	UPROPERTY(BlueprintAssignable)
	FOnInteractChangedSignature OnInteractChanged;

	UPROPERTY(BlueprintAssignable)
	FOnInteractedSignature OnInteracted;

	UPROPERTY(BlueprintAssignable)
	FOnInteractedSignature OnInteractCanceled;
};
