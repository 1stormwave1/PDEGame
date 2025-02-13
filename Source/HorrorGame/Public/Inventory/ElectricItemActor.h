

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemActor.h"
#include "ElectricItemActor.generated.h"


UCLASS()
class HORRORGAME_API AElectricItemActor : public AItemActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBurnOut = false;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Feedback Loop")
	void StartFeedbackLoop();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Feedback Loop")
	void FeedbackLoopTick();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Feedback Loop")
	void EndFeedbackLoop();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Overcharge")
	void StartOvercharge();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Overcharge")
	void EndOvercharge();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Overcharge")
	void BurnOut();

	virtual void SetItemActorParams_Implementation(UItem* Item) override;

	UFUNCTION()
	void OnBurnOutChanged(bool bBurnOut);
};
