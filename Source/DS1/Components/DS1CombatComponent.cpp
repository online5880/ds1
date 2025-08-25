#include "DS1CombatComponent.h"

#include "Character/DS1Character.h"
#include "Equipments/DS1Weapon.h"
#include "Items/DS1PickupItem.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1CombatComponent)

UDS1CombatComponent::UDS1CombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UDS1CombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDS1CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UDS1CombatComponent::SetWeapon(ADS1Weapon* NewWeapon)
{
	if (::IsValid(MainWeapon))
	{
		if (const ADS1Character* OwnerCharacter = Cast<ADS1Character>(GetOwner()))
		{
			ADS1PickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ADS1PickupItem>(ADS1PickupItem::StaticClass(), OwnerCharacter->GetTransform(), nullptr, nullptr , ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			PickupItem->SetEquipmentClass(MainWeapon->GetClass());
			PickupItem->FinishSpawning(GetOwner()->GetActorTransform());

			MainWeapon->Destroy();
		}
	}
	MainWeapon = NewWeapon;
}

void UDS1CombatComponent::SetCombatEnabled(const bool bEnabled)
{
	bCombatEnabled = bEnabled;
	if (OnChangedCombat.IsBound())
	{
		OnChangedCombat.Broadcast(bCombatEnabled);
	}
}

