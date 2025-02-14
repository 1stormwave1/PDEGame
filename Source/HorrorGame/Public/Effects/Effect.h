

#pragma once

#include "CoreMinimal.h"
#include "Effect.generated.h"


class AHorrorPlayerState;
class AHorrorGameState;
class UEffectComponent;
class UEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectAppliedSignature, UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectRemovedSignature, UEffect*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImmunityEffectAddedSignature, UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImmunityEffectRemovedSignature, UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImmunityEffectTriggeredSignature, UEffect*, Effect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChangedSignature, int32, Stack);

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	Neutral,
	Buff,
	Debuff
};

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UEffect : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UEffectComponent> EffectComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEffectType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stacks")
	bool bIsStackable = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stacks", meta= (EditCondition = "bIsStackable"))
	TArray<float> StacksValues;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentStack = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Duration")
	bool bIsInfinite = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Duration", meta = (EditCondition = "!bIsInfinite"))
	float Duration = 3.f;

	bool bIsApplied = false;

	FTimerHandle DurationHandle;

public:
	UPROPERTY(BlueprintAssignable)
	FEffectAppliedSignature OnEffectApplied;

	UPROPERTY(BlueprintAssignable)
	FEffectRemovedSignature OnEffectRemoved;

	UPROPERTY(BlueprintAssignable)
	FStackChangedSignature OnStackChanged;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetStackValue() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeStacksAmount(int32 StacksDelta);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetEffectComponent(UEffectComponent* NewEffectComponent) { EffectComponent = NewEffectComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FText GetEffectName() const { return Name; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }

	UFUNCTION(BlueprintCallable)
	AHorrorGameState* GetGameState() const;

	UFUNCTION(BlueprintCallable)
	AHorrorPlayerState* GetPlayerState() const;

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable)
	void ClearAndInvalidateTimer(const FTimerHandle& TimerHandle);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnActorFromClass(TSubclassOf<AActor> ActorClass);
};
