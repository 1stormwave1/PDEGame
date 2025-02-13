

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentHealthChangedSignature, float, NewHealthStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiedSignature);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;
	
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FCurrentHealthChangedSignature OnCurrentHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FDiedSignature OnDied;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	void ChangeCurrentHealth(float DeltaHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();
	
};
