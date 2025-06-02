
#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FVector Velocity;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float GroundSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsFalling;

public:
	UDS1AnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
