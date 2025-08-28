#include "DS1Weapon.h"

#include "DS1GameplayTags.h"
#include "Animation/DS1AnimInstance.h"
#include "Character/DS1Character.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1WeaponCollisionComponent.h"
#include "Data/DS1MontageActionData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1Weapon)

ADS1Weapon::ADS1Weapon()
{
	WeaponCollision = CreateDefaultSubobject<UDS1WeaponCollisionComponent>(TEXT("WeaponCollision"));
	WeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

	SecondWeaponCollision = CreateDefaultSubobject<UDS1WeaponCollisionComponent>(TEXT("SecondCollision"));
	SecondWeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);
	
	StaminaCostMap.Emplace(DS1GameplayTags::Character_Attack_Light, 7.f);
	StaminaCostMap.Emplace(DS1GameplayTags::Character_Attack_Heavy, 20.f);
	StaminaCostMap.Emplace(DS1GameplayTags::Character_Attack_Special, 15.f);
	StaminaCostMap.Emplace(DS1GameplayTags::Character_Attack_Running,12.f);

	DamageMultiplierMap.Emplace(DS1GameplayTags::Character_Attack_Heavy, 1.8f);
	DamageMultiplierMap.Emplace(DS1GameplayTags::Character_Attack_Running, 1.8f);
	DamageMultiplierMap.Emplace(DS1GameplayTags::Character_Attack_Special, 2.1f);
}

void ADS1Weapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<UDS1CombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;

		AttachToOwner(AttachSocket);

		// 무기의 충돌 트레이스 컴포넌트에 무기 메시를 설정
		WeaponCollision->SetWeaponMesh(Mesh);

		// 장착한 무기의 CombatTyp으로 업데이트
		if (const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (UDS1AnimInstance* Anim = Cast<UDS1AnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}
		}
		// 무기를 소유한 OwnerActor를 충돌에서 제외
		WeaponCollision->AddIgnoreActor(GetOwner());
	}
}

UAnimMontage* ADS1Weapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

UAnimMontage* ADS1Weapon::GetRandomMontageForTag(const FGameplayTag& Tag) const
{
	return MontageActionData->GetRandomMontageForTag(Tag);
}

UAnimMontage* ADS1Weapon::GetHitReactMontage(const AActor* Attacker) const
{
	// LookAt 회전값 (현재 Actor가 공격자를 바라보는 회전값)
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	// 현재 Actor의 회전값과 LookAt 회전값의 차이
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	// Z축 회전값
	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitDirection = EHitDirection::Right;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Left;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.f, 180.f) || UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
	}

	UAnimMontage* SelectMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectMontage = GetMontageForTag(DS1GameplayTags::Character_Action_HitReaction, 0);
		break;
	case EHitDirection::Back:
		SelectMontage = GetMontageForTag(DS1GameplayTags::Character_Action_HitReaction, 1);
		break;
	case EHitDirection::Left:
		SelectMontage = GetMontageForTag(DS1GameplayTags::Character_Action_HitReaction, 2);
		break;
	case EHitDirection::Right:
		SelectMontage = GetMontageForTag(DS1GameplayTags::Character_Action_HitReaction, 3);
		break;
	default:
		break;
	}

	return SelectMontage;
}

float ADS1Weapon::GetStaminaCostForTag(const FGameplayTag& Tag) const
{
	if (StaminaCostMap.Contains(Tag))
	{
		return StaminaCostMap[Tag];
	}
	return 0.0f;
}

float ADS1Weapon::GetAttackDamage()
{
	if (const AActor* OwnerActor = GetOwner())
	{
		const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();

		if (DamageMultiplierMap.Contains(LastAttackType))
		{
			const float Multiplier = DamageMultiplierMap[LastAttackType];
			return BaseDamage * Multiplier;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("ADS1Weapon::GetAttackDamage - OwnerActor가 null입니다."));
	return BaseDamage; // 기본 데미지 반환
}

void ADS1Weapon::ActivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOnCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOnCollision();
		break;
	default: ;
	}
}

void ADS1Weapon::DeactivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOffCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOffCollision();
		break;
	default: ;
	}
}

void ADS1Weapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();

	// 데미지 방향
	const FVector DamageDirection = GetOwner()->GetActorForwardVector();

	// 데미지
	const float AttackDamage = GetAttackDamage();

	UGameplayStatics::ApplyPointDamage(
		TargetActor,
		AttackDamage,
		DamageDirection,
		Hit,
		GetOwner()->GetInstigatorController(),
		this,
		nullptr);
}
