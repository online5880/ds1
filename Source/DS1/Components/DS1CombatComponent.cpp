// Fill out your copyright notice in the Description page of Project Settings.


#include "DS1CombatComponent.h"

#include "Character/DS1Character.h"
#include "Equipments/DS1Weapon.h"
#include "Items/DS1PickupItem.h"

UCombatComponent::UCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::SetWeapon(ADS1Weapon* NewWeapon)
{
	if (::IsValid(MainWeapon))
	{
		if (ADS1Character* OwnerCharacter = Cast<ADS1Character>(GetOwner()))
		{
			ADS1PickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ADS1PickupItem>(ADS1PickupItem::StaticClass(), OwnerCharacter->GetTransform(), nullptr, nullptr , ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			PickupItem->SetEquipmentClass(MainWeapon->GetClass());
			PickupItem->FinishSpawning(GetOwner()->GetActorTransform());

			MainWeapon->Destroy();
		}
	}
	MainWeapon = NewWeapon;
}

