// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "DS1Equipment.h"
#include "GameplayTagContainer.h"
#include "DS1Weapon.generated.h"

class UDS1WeaponCollisionComponent;
class UDS1MontageActionData;

UCLASS()
class DS1_API ADS1Weapon : public ADS1Equipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	UDS1MontageActionData* MontageActionData;

// Component Section
protected:
	UPROPERTY(VisibleAnywhere)
	UDS1WeaponCollisionComponent* WeaponCollisionComponent;
	
	UPROPERTY()
	class UDS1CombatComponent* CombatComponent;

protected:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

	// 기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Damage")
	float BaseDamage = 15.f;

	// 데미지 승수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Damage")
	TMap<FGameplayTag, float> DamageMultiplierMap;
 	

public:
	ADS1Weapon();

public:
	virtual void EquipItem() override;

	UAnimMontage* GetMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;

	float GetStaminaCostForTag(const FGameplayTag& Tag) const;
	float GetAttackDamage();

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }
	FORCEINLINE UDS1WeaponCollisionComponent* GetWeaponCollisionComponent() const { return WeaponCollisionComponent; }

public:
	// 무기의 Collision에 검출된 액터에 대한 데미지를 적용
	void OnHitActor(const FHitResult& Hit);
};
