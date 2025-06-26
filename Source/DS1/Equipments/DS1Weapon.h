// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS1Equipment.h"
#include "DS1Weapon.generated.h"

struct FGameplayTag;
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

protected:
	UPROPERTY()
	class UDS1CombatComponent* CombatComponent;

public:
	ADS1Weapon();

public:
	virtual void EquipItem() override;

	UAnimMontage* GetMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }
};
