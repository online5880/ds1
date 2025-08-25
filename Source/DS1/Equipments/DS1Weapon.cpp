#include "DS1Weapon.h"

#include "DS1GameplayTags.h"
#include "Animation/DS1AnimInstance.h"
#include "Character/DS1Character.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1WeaponCollisionComponent.h"
#include "Data/DS1MontageActionData.h"
#include "Kismet/GameplayStatics.h"

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
		if (ADS1Character* OwnerCharacter = Cast<ADS1Character>(GetOwner()))
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
