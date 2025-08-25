#pragma once

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DS1CombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateOnChangedCombat,bool);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DS1_API UDS1CombatComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	FDelegateOnChangedCombat OnChangedCombat;

protected:
	UPROPERTY()
	class ADS1Weapon* MainWeapon;

	/* 전투 활성화 상태인지? */
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;

	/* 마지막 공격 타입 */
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

public:
	UDS1CombatComponent();

protected:

	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeapon(ADS1Weapon* NewWeapon);
	
public:
	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	void SetCombatEnabled(const bool bEnabled);
	
	FORCEINLINE ADS1Weapon* GetMainWeapon() const { return MainWeapon; }

	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; }
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackType) { LastAttackType = NewAttackType; }
};
