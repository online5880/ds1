#pragma once

#include "DS1Define.h"
#include "UObject/Interface.h"
#include "DS1CombatInterface.generated.h"

struct FGameplayTag;

UINTERFACE()
class UDS1CombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DS1_API IDS1CombatInterface
{
	GENERATED_BODY()

	
public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) = 0;
	
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) = 0;

	virtual void PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) {}
};
