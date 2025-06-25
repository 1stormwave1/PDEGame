

#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

class AHorrorGameState;
class UFillUpItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBurnOutChangedSignature, bool, bNewBurnOut);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerChangedSignature, float, NewPower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBurnOutPowerChangedSignature, bool, bNewBurnOut);

class AItemActor;

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnBurnOutChangedSignature OnBurnOutChanged;

	UPROPERTY(BlueprintAssignable)
	FOnBurnOutPowerChangedSignature OnBurnOutPowerChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPowerChangedSignature OnPowerChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Description")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Description")
	TMap<FString, float> FormatArgs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBurnOut = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsConsumable = true;

	UPROPERTY(BlueprintReadWrite)
	bool bIsBurnOutPower = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	bool bIsLimitedPower = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float MaxPower = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float CurrentPower = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float UseTime = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float TickTime = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	TArray<TSubclassOf<UFillUpItem>> FillUpItems;

	FTimerHandle PowerTimerHandle;

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable)
	void StartUsePower();

	UFUNCTION(BlueprintCallable)
	void StopUsePower();

	UFUNCTION(BlueprintNativeEvent)
	void TickPowerTimer();

	UFUNCTION(BlueprintCallable)
	void BurnOutPower();

	UFUNCTION(BlueprintCallable)
	void FillUpPower(const TArray<UFillUpItem*>& Items);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisactivateItem();

	UFUNCTION(BlueprintPure)
	FText GetFormattedDescription();
	
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintNativeEvent)
	void SetFormatArgs();

	UFUNCTION(BlueprintPure)
	AHorrorGameState* GetGameState() const;

	UFUNCTION(BlueprintPure)
	AHorrorPlayerState* GetPlayerState() const;

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsConsumable() const { return bIsConsumable; }
};
