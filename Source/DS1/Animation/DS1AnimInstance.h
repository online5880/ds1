
#pragma once

#include "CoreMinimal.h"
#include "DS1Define.h"
#include "Animation/AnimInstance.h"
#include "DS1AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UDS1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	ACharacter* Character;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UCharacterMovementComponent* MovementComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Data")
	FVector Velocity;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float GroundSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float Direction;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Data")
	bool bCombatEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Data")
	ECombatType CombatType = ECombatType::None;

public:
	UDS1AnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void AnimNotify_ResetMovementInput();

	UFUNCTION()
	void AnimNotify_ResetState();

public:
	void UpdateCombatMode(const ECombatType InCombatType);

protected:
	void OnChangedCombat(const bool bInCombatEnabled);
};
