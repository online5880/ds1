
#include "Character/DS1Character.h"

#include "DS1GameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1AttributeActorComponent.h"
#include "Components/DS1StateComponent.h"
#include "Components/DS1TargetingComponent.h"
#include "Equipments/DS1Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/DS1Interact.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/DS1PlayerHUDWidget.h"


ADS1Character::ADS1Character()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	/** 이동, 감속 속도*/
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetRelativeRotation(FRotator(-30.f, 0.0f, 0.0f));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<UDS1AttributeActorComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<UDS1StateComponent>(TEXT("State"));
	CombatComponent = CreateDefaultSubobject<UDS1CombatComponent>(TEXT("Combat"));

	// LockedOn Targeting
	TargetingComponent = CreateDefaultSubobject<UDS1TargetingComponent>(TEXT("Targeting"));
}

void ADS1Character::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UDS1PlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}
}

void ADS1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Stamina: %f"), AttributeComponent->GetBaseStamina()));
	// GEngine->AddOnScreenDebugMessage(2, 1.5f, FColor::Cyan, FString::Printf(TEXT("MaxWalkSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed));
}

void ADS1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext,0);
		}
	}
}

void ADS1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADS1Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADS1Character::Look);

		// 스프린트 액션 바인딩
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ThisClass::Sprinting);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);

		// 구르기 액션 바인딩
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ThisClass::Rolling);

		// 상호작용 액션 바인딩
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);

		// 전투 활성화/비활성화 토글
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ThisClass::ToggleCombat);

		// 공격 상태로 자동 전환
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AutoToggleCombat);
		// 일반 공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ThisClass::Attack);
		// 특수 공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADS1Character::SpecialAttack);
		// 강력한 공격
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &ADS1Character::HeavyAttack);

		// LockOn 관련 액션 바인딩
		EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Started, this, &ADS1Character::LockOnTarget);
		EnhancedInputComponent->BindAction(LeftTargetAction, ETriggerEvent::Started, this, &ADS1Character::LeftTarget);
		EnhancedInputComponent->BindAction(RightTargetAction, ETriggerEvent::Started, this, &ADS1Character::RightTarget);
	}
}

bool ADS1Character::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		return MovementComp->Velocity.Size2D() > 3.f && MovementComp->GetCurrentAcceleration() != FVector::ZeroVector;
	}
	return false;
}

bool ADS1Character::CanToggleCombat() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(DS1GameplayTags::Character_State_Attacking);
	CheckTags.AddTag(DS1GameplayTags::Character_State_Rolling);
	CheckTags.AddTag(DS1GameplayTags::Character_State_GeneralAction);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}


void ADS1Character::Move(const FInputActionValue& Values)
{
	check(StateComponent)

	// 이동 입력 가능 상태인지 체크
	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}
	
	FVector2D MovementVector = Values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void ADS1Character::Look(const FInputActionValue& Values)
{
	// Locked On 상태에서 Look 입력을 처리하지 않음
	if (TargetingComponent && TargetingComponent->IsLockOn())
	{
		return;
	}

	const FVector2D LookVector = Values.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const float YawInput = LookVector.X;
		const float PitchInput = LookVector.Y;

		AddControllerYawInput(YawInput);
		AddControllerPitchInput(PitchInput*-1.0f); // Invert pitch input for a more natural feel
	}
}

void ADS1Character::Sprinting()
{
	if (AttributeComponent->CheckHasEnoughStamina(5.f) && IsMoving())
	{
		AttributeComponent->ToggleStaminaRegeneration(false); // 스프린트 중에는 스태미나 재충전 비활성화
		
		// 스프린트 중일 때 속도를 증가시키고 스태미나 감소
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		AttributeComponent->DecreaseStamina(0.1f); // 스프린트 시 스태미나 감소

		bSprinting = true;
	}
	else
	{
		StopSprint();
	}
}

void ADS1Character::StopSprint()
{
	// 스프린트 중지 시 속도를 원래대로 되돌림
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttributeComponent->ToggleStaminaRegeneration(true);
	bSprinting = false;
}

void ADS1Character::Rolling()
{
	check(AttributeComponent)
	check(StateComponent)

	if (AttributeComponent->CheckHasEnoughStamina(15.f))
	{
		// 스태미나 재충전 멈춤
		AttributeComponent->ToggleStaminaRegeneration(false);

		// 이동입력 처리 무시
		StateComponent->ToggleMovementInput(false);

		// 스태미나 차감
		AttributeComponent->DecreaseStamina(15.f);

		// 구르기 애니메이션 재생
		PlayAnimMontage(RollingMontage);

		StateComponent->SetState(DS1GameplayTags::Character_State_Rolling);

		// 스태미나 재충전 시작
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ADS1Character::Interact()
{
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	constexpr float Radius = 100.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		Radius,
		ObjectTypes,
		false,
		ActorToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true);

	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (IDS1Interact* Interaction = Cast<IDS1Interact>(HitActor))
			{
				// 상호작용 인터페이스가 구현되어 있다면 상호작용 호출
				Interaction->Interact(this);
			}
		}
	}
} 

void ADS1Character::ToggleCombat()
{
	check(CombatComponent)
	check(StateComponent)

	if (CombatComponent)
	{
		if (const ADS1Weapon* Weapon = CombatComponent->GetMainWeapon())
		{
			if (CanToggleCombat())
			{
				StateComponent->SetState(DS1GameplayTags::Character_State_GeneralAction);

				if (CombatComponent->IsCombatEnabled())
				{
					PlayAnimMontage(Weapon->GetMontageForTag(DS1GameplayTags::Character_Action_Unequip));
				}
				else
				{
					PlayAnimMontage(Weapon->GetMontageForTag(DS1GameplayTags::Character_Action_Equip));
				}
			}
		}
	}
	
}

void ADS1Character::AutoToggleCombat()
{
	if (CombatComponent)
	{
		if (!CombatComponent->IsCombatEnabled())
		{
			ToggleCombat();
		}
	}
}

void ADS1Character::Attack()
{
	const FGameplayTag AttackTypeTag = GetAttackPerform();

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ADS1Character::SpecialAttack()
{
	const FGameplayTag AttackTypeTag = DS1GameplayTags::Character_Attack_Special;

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ADS1Character::HeavyAttack()
{
	AutoToggleCombat();

	const FGameplayTag AttackTypeTag = DS1GameplayTags::Character_Attack_Heavy;

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ADS1Character::LockOnTarget()
{
	TargetingComponent->ToggleLockOn();
}

void ADS1Character::LeftTarget()
{
	if (TargetingComponent)
	{
		TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Left);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetingComponent is not valid!"));
	}
}

void ADS1Character::RightTarget()
{
	if (TargetingComponent)
	{
		TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Right);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetingComponent is not valid!"));
	}
}


FGameplayTag ADS1Character::GetAttackPerform() const
{
	if (IsSprinting())
	{
		return DS1GameplayTags::Character_Attack_Running;
	}
	return DS1GameplayTags::Character_Attack_Light;
}

bool ADS1Character::CanPerformAttack(const FGameplayTag& AttackType) const
{
	check(StateComponent)
	check(CombatComponent)
	check(AttributeComponent)

	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(DS1GameplayTags::Character_State_Rolling);
	CheckTags.AddTag(DS1GameplayTags::Character_State_GeneralAction);

	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCostForTag(AttackType);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false
			&& CombatComponent->IsCombatEnabled()
			&& AttributeComponent->CheckHasEnoughStamina(StaminaCost);
}

void ADS1Character::DoAttack(const FGameplayTag& AttackType)
{
	check(StateComponent)
	check(CombatComponent)
	check(AttributeComponent)

	if (const ADS1Weapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(DS1GameplayTags::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false);
		CombatComponent->SetLastAttackType(AttackType);

		AttributeComponent->ToggleStaminaRegeneration(false); // 공격 중 스태미나 재충전 비활성화

		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackType, ComboCounter);
		if (!Montage)
		{
			// 콤보 한계 도달
			ComboCounter = 0;
			Montage = Weapon->GetMontageForTag(AttackType, ComboCounter);
		}

		PlayAnimMontage(Montage);

		const float StaminaCost = Weapon->GetStaminaCostForTag(AttackType);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ADS1Character::ExecuteComboAttack(const FGameplayTag& AttackType)
{
	if (StateComponent->GetCurrentState() != DS1GameplayTags::Character_State_Attacking)
	{
		if (bComboSequenceRunning && bCanComboInput == false)
		{
			// 애니메이션은 끝났지만 아직 콤보 시퀀스가 유요할 때 - 추가 입력 기회
			ComboCounter++;
			UE_LOG(LogTemp, Warning, TEXT("Combo Counter: %d"), ComboCounter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Combo Sequence Started"));
			ResetCombo();
			bComboSequenceRunning = true;
		}
		DoAttack(AttackType);
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	else if (bCanComboInput)
	{
		// 콤보 윈도우가 열려 있을 때 - 최적의 타이밍
		bSavedComboInput = true;
	}
}

void ADS1Character::ResetCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Reset"));

	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

void ADS1Character::EnableComboWindow()
{
	bCanComboInput = true;
	UE_LOG(LogTemp, Warning, TEXT("Combo Window Enabled"));
}

void ADS1Character::DisableComboWindow()
{
	check(CombatComponent)

	bCanComboInput = false;

	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCounter++;
		UE_LOG(LogTemp, Warning, TEXT("Combo Counter: %d"), ComboCounter);
		DoAttack(CombatComponent->GetLastAttackType());
	}
}

void ADS1Character::AttackFinished(const float ComboResetDelay)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Finished"));
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}
	// ComboResetDelay 후에 콤보 시퀀스 종료
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}
