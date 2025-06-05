#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DS1CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DS1_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class ADS1Weapon* MainWeapon;

public:
	UCombatComponent();

protected:

	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeapon(ADS1Weapon* NewWeapon);
	
};
