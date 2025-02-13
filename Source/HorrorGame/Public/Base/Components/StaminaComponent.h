

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentStaminaChangedSignature, float, NewCurrentStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxStaminaChangedSignature, float, NewMaxStamina);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float CurrentStamina = 100.f;

	UPROPERTY(EditAnywhere)
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Decrease")
	float DecreasePerTick = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Decrease")
	float DecreaseTickTime = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Increase")
	float IncreasePerTick = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Increase")
	float IncreaseTickTime = 0.1f;

	float CurrentTickDelta = 0.f;

	UPROPERTY(EditAnywhere)
	float RestTime = 3.f;

	FTimerHandle DecreaseTickHandle;
	FTimerHandle IncreaseTickHandle;
	FTimerHandle RestTimerTickHandle;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FCurrentStaminaChangedSignature CurrentStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FMaxStaminaChangedSignature MaxStaminaChangedSignature;

	UPROPERTY(EditAnywhere)
	bool bUseStamina = true;

	UPROPERTY(BlueprintReadOnly)
	bool bIsUsingStamina = false;

	UFUNCTION()
	void ChangeStaminaTick();

	UFUNCTION()
	void StartIncrease();

	UFUNCTION(BlueprintCallable)
	void StartUseStamina();

	UFUNCTION(BlueprintCallable)
	void StopUseStamina();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintCallable)
	void SetMaxStamina(const float NewMaxStamina); 
	
};
