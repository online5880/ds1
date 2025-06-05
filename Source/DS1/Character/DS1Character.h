// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DS1Character.generated.h"


struct FInputActionValue;

UCLASS()
class DS1_API ADS1Character : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintRollingAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UDS1AttributeActorComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UDS1StateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* CombatComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	class UDS1PlayerHUDWidget* PlayerHUDWidget;

protected:
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float SprintSpeed = 750.f; // 스프린트 속도

	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float NormalSpeed = 500.f; // 일반 속도

protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* RollingMontage; // 구르기 애니메이션 몽타주
	
public:
	ADS1Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE UDS1StateComponent* GetStateComponent() const { return StateComponent; }

protected:
	/** 캐릭터가 이동중인지 체크 */
	bool IsMoving() const;

	/** 이동 */
	void Move(const FInputActionValue& Values);
	/** 카메라 방향 */
	void Look(const FInputActionValue& Values);
	/** 질주 */
	void Sprinting();
	/** 질주 중지 */
	void StopSprint();
	/** 구르기 */
	void Rolling();
	/** 상호작용 */
	void Interact();
};


