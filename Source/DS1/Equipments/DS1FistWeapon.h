#pragma once

#include "DS1Weapon.h"
#include "DS1FistWeapon.generated.h"

UCLASS()
class DS1_API ADS1FistWeapon : public ADS1Weapon
{
	GENERATED_BODY()

public:
	ADS1FistWeapon();
	
public:
	virtual void EquipItem() override;

};
