#include "DS1Enemy.h"

#include "DS1GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Components/DS1AttributeActorComponent.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1StateComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Equipments/DS1Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1Enemy)

ADS1Enemy::ADS1Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Targeting 구체 생성 및 Collision 설정
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphere"));
	TargetingSphereComponent->SetupAttachment(RootComponent);
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Lock On UI 위젯 컴포넌트 생성
	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(RootComponent);
	LockOnWidgetComponent->SetRelativeLocation(FVector(0.f,0.f,50.f));
	LockOnWidgetComponent->SetDrawSize(FVector2D(30.f,30.f));
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<UDS1AttributeActorComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<UDS1StateComponent>(TEXT("State"));
	CombatComponent = CreateDefaultSubobject<UDS1CombatComponent>(TEXT("Combat"));

	// OnDeath 델리게이트 바인딩
	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
}

void ADS1Enemy::BeginPlay()
{
	Super::BeginPlay();

	// 무기 장착
	if (DefaultWeaponClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;

		ADS1Weapon* Weapon = GetWorld()->SpawnActor<ADS1Weapon>(DefaultWeaponClass,GetActorTransform(),Params);
		CombatComponent->SetCombatEnabled(true);
		Weapon->EquipItem();
	}
}

void ADS1Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ADS1Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Enemy Take Damage: %f"), ActualDamage));
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		// 데미지 방향
		FVector ShotDirection = PointDamageEvent->ShotDirection;

		// 히트 위치(표면 접촉 관점)
		FVector ImpactLocation = PointDamageEvent->HitInfo.ImpactPoint;

		// 히트 방향
		FVector HitNormal = PointDamageEvent->HitInfo.ImpactNormal;

		// 히트한 객체의 Location(객체 중심 관점)
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactLocation);
		
		HitReact(EventInstigator->GetPawn());
	}

	return ActualDamage;
}

void ADS1Enemy::OnDeath()
{
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Ragdoll
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
}

void ADS1Enemy::ImpactEffect(const FVector& Location)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
	}
	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
	}
}

void ADS1Enemy::HitReact(const AActor* Attacker)
{
	check(CombatComponent)
	
	if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetHitReactMontage(Attacker))
	{
		PlayAnimMontage(HitReactAnimMontage);
	}
}

void ADS1Enemy::OnTargeted(bool bTargeted)
{
	if (LockOnWidgetComponent)
	{
		LockOnWidgetComponent->SetVisibility(bTargeted);
	}
}

bool ADS1Enemy::CanBeTargeted()
{
	if (!StateComponent) return false;

	FGameplayTagContainer TagCheck;
	TagCheck.AddTag(DS1GameplayTags::Character_State_Death);
	return StateComponent->IsCurrentStateEqualToAny(TagCheck) == false;
	
}

void ADS1Enemy::ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->ActivateCollision(WeaponCollisionType);
	}
}

void ADS1Enemy::DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->ActivateCollision(WeaponCollisionType);
	}
}

void ADS1Enemy::PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate)
{
	check(StateComponent)
	check(AttributeComponent)
	check(CombatComponent)

	if (const ADS1Weapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(DS1GameplayTags::Character_State_Attacking);
		CombatComponent->SetLastAttackType(AttackTypeTag);
		AttributeComponent->ToggleStaminaRegeneration(false);

		if (UAnimMontage* Montage = Weapon->GetRandomMontageForTag(AttackTypeTag))
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(Montage);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
			}
		}

		const float StaminaCost = Weapon->GetStaminaCostForTag(AttackTypeTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

