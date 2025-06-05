
#include "Character/DS1Character.h"

#include "DS1GameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1AttributeActorComponent.h"
#include "Components/DS1StateComponent.h"
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
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
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

		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ThisClass::Sprinting);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ThisClass::Rolling);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
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
	FVector2D LookVector = Values.Get<FVector2D>();
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

