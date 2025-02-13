

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Effects/Effect.h"
#include "EffectComponent.generated.h"


class UEffect;
class AHorrorGameCharacter;

USTRUCT(BlueprintType)
struct FCurrentEffect
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Transient)
	TArray<UEffect*> SameClassEffects;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentStack = 0;

	void Remove();

	void Apply();
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<AHorrorGameCharacter> HorrorGameCharacter = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient)
	TArray<UEffect*> CurrentEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UEffect>> ImmunityEffects;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEffectRemoved(UEffect* Effect);

public:
	UPROPERTY(BlueprintAssignable)
	FEffectAppliedSignature OnEffectAppliedDelegate;

	UPROPERTY(BlueprintAssignable)
	FEffectAppliedSignature OnEffectReappliedDelegate;

	UPROPERTY(BlueprintAssignable)
	FEffectRemovedSignature OnEffectRemovedDelegate;

	UPROPERTY(BlueprintAssignable)
	FImmunityEffectAddedSignature OnImmunityEffectAddedDelegate;

	UPROPERTY(BlueprintAssignable)
	FImmunityEffectRemovedSignature OnImmunityEffectRemovedDelegate;

	UPROPERTY(BlueprintAssignable)
	FImmunityEffectTriggeredSignature OnImmunityEffectTriggeredDelegate;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyEffectByClass(TSubclassOf<UEffect> EffectClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddImmunityEffect(TSubclassOf<UEffect> EffectClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveImmunityEffect(TSubclassOf<UEffect> EffectClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveEffectByClass(TSubclassOf<UEffect> EffectClass);

	
};
