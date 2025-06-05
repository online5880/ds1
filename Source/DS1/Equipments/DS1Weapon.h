// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS1Equipment.h"
#include "DS1Weapon.generated.h"

UCLASS()
class DS1_API ADS1Weapon : public ADS1Equipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;

protected:
	UPROPERTY()
	class UCombatComponent* CombatComponent;

public:
	ADS1Weapon();

public:
	virtual void EquipItem() override;
};
