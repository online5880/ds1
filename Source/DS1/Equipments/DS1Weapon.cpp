#include "DS1Weapon.h"

#include "Components/DS1CombatComponent.h"

ADS1Weapon::ADS1Weapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADS1Weapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<UCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		AttachToOwner(UnequipSocketName);
	}
}
