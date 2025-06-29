#include "DS1Weapon.h"

#include "DS1GameplayTags.h"
#include "Components/DS1CombatComponent.h"
#include "Data/DS1MontageActionData.h"

ADS1Weapon::ADS1Weapon()
{
	StaminaCostMap.Emplace(DS1GameplayTags::Character_Attack_Light, 7.f);
	StaminaCostMap.Emplace(DS1GameplayTags::Character_Attack_Heavy, 20.f);
	StaminaCostMap.Emplace(DS1GameplayTags::Character_Attack_Special, 15.f);
	StaminaCostMap.Emplace(DS1GameplayTags::Character_Attack_Running,12.f);
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
