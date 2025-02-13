

#include "HorrorGame/Public/Base/HorrorGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Base/HorrorGameController.h"
#include "Base/Components/EffectComponent.h"
#include "Base/Components/HealthComponent.h"
#include "Base/Components/InventoryComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorGame/Public/Base/Components/StaminaComponent.h"


AHorrorGameCharacter::AHorrorGameCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EffectComponent = CreateDefaultSubobject<UEffectComponent>(TEXT("EffectComponent"));
	ItemAttachPointComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ItemAttachPointComponent"));
	ItemAttachPointComponent->SetupAttachment(FirstPersonCameraComponent);
}

void AHorrorGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AHorrorGameController* PlayerController = Cast<AHorrorGameController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AHorrorGameCharacter::MoveEnd(const FInputActionValue& Value)
{
	if(StaminaComponent->bIsUsingStamina)
	{
		StaminaComponent->StopUseStamina();
	}
}

void AHorrorGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AHorrorGameCharacter::StopCrouch);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AHorrorGameCharacter::MoveEnd);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHorrorGameCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHorrorGameCharacter::StopSprinting);
	}
}

void AHorrorGameCharacter::UpdateSpeedCoefficient(const float NewSpeedCoefficient)
{
	ClearSpeedCoefficient();
	
	SpeedCoefficient = NewSpeedCoefficient;
	StartWalkSpeed *= SpeedCoefficient;
	SprintSpeed *= SpeedCoefficient;
	GetCharacterMovement()->MaxWalkSpeed *= SpeedCoefficient;
	GetCharacterMovement()->MaxWalkSpeedCrouched *= SpeedCoefficient;
}

void AHorrorGameCharacter::ClearSpeedCoefficient()
{
	StartWalkSpeed /= SpeedCoefficient;
	SprintSpeed /= SpeedCoefficient;
	GetCharacterMovement()->MaxWalkSpeed /= SpeedCoefficient;
	GetCharacterMovement()->MaxWalkSpeedCrouched /= SpeedCoefficient;
	SpeedCoefficient = 1.f;
}

float AHorrorGameCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                                       AActor* DamageCauser)
{
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AHorrorGameCharacter::Destroyed()
{
	if (AHorrorGameController* PlayerController = Cast<AHorrorGameController>(Controller))
	{
		PlayerController->GetInventoryComponent()->DestroyActiveSlotItem();
	}
	Super::Destroyed();
}

void AHorrorGameCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y * MovementCoefficient);
		AddMovementInput(GetActorRightVector(), MovementVector.X * MovementCoefficient);
		
		if(bIsSprinting)
		{
			if(!StaminaComponent->bIsUsingStamina)
			{
				StaminaComponent->StartUseStamina();
			}
			if(StaminaComponent->GetCurrentStamina() <= 0.f)
			{
				StopSprinting();
			}
		}

		MoveBP();
	}
}

void AHorrorGameCharacter::MoveBP_Implementation()
{
}

void AHorrorGameCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * MovementCoefficient);
		AddControllerPitchInput(LookAxisVector.Y * MovementCoefficient);
	}
}

void AHorrorGameCharacter::StartCrouch()
{
	if(!bIsSprinting)
	{
		Super::Crouch(true);
	}
}

void AHorrorGameCharacter::StopCrouch()
{
	if(!bIsSprinting)
	{
		Super::UnCrouch(true);
	}
}

void AHorrorGameCharacter::StartSprinting_Implementation()
{
	if(!bIsCrouched)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AHorrorGameCharacter::StopSprinting_Implementation()
{
	if(bIsSprinting && !bIsCrouched)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = StartWalkSpeed;

		if(StaminaComponent->bIsUsingStamina)
		{
			StaminaComponent->StopUseStamina();
		}
	}
}
