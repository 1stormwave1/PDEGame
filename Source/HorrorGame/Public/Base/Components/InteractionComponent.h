
#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"


class UItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractChangedSignature, bool, bIsInteractable);

class AHorrorGameController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(Transient)
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
	
	UFUNCTION(BlueprintNativeEvent)
	void Interact();

	UPROPERTY(BlueprintAssignable)
	FOnInteractChangedSignature OnInteractChanged;
};
