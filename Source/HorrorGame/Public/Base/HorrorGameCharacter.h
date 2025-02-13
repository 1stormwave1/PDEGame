

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HorrorGameCharacter.generated.h"

class UEffectComponent;
class UHealthComponent;
class UStaminaComponent;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AHorrorGameCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ItemAttachPointComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaminaComponent* StaminaComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEffectComponent* EffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sprint)
	float SprintSpeed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sprint)
	float StartWalkSpeed = 400.f;

	UPROPERTY(BlueprintReadWrite)
	float SpeedCoefficient = 1.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSprinting = false;
	
	virtual void BeginPlay();

	void MoveEnd(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void MoveBP();
	
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category=Character)
	void StartCrouch();

	UFUNCTION(BlueprintCallable, Category=Character)
	void StopCrouch();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Character)
	void StartSprinting();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Character)
	void StopSprinting();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	UPROPERTY(BlueprintReadWrite)
	float MovementCoefficient = 1.f;

	UFUNCTION(BlueprintCallable)
	void UpdateSpeedCoefficient(const float NewSpeedCoefficient = 1.f);
	
	UFUNCTION(BlueprintCallable)
	void ClearSpeedCoefficient();
	
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void Destroyed() override;

	AHorrorGameCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	FORCEINLINE UArrowComponent* GetItemAttachPointComponent() const { return ItemAttachPointComponent; }

	
};

