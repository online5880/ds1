// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
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

	/* 전투 활성화/비활성화 토글 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleCombatAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* HeavyAttackAction;

	// Locked On
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnTargetAction;
	
	// 왼쪽으로 타겟 전환
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftTargetAction;

	// 오른쪽으로 타겟 전환
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RightTargetAction;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UDS1AttributeActorComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UDS1StateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UDS1CombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UDS1TargetingComponent* TargetingComponent;
	
// UI Section
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

	UPROPERTY(VisibleAnywhere, Category = "Sprinting")
	bool bSprinting = false; // 스프린트 중인지 여부


// Combo Section
protected:
	/* 콤보 시퀀스 진행중 */
	bool bComboSequenceRunning = false;

	/* 콤보 입력 가능? */
	bool bCanComboInput = false;

	/* 콤보 카운터 */
	int32 ComboCounter = 0;

	/* 콤보 입력 여부 */
	bool bSavedComboInput = false;

	/* 콤보 리셋 타이머 핸들 */
	FTimerHandle ComboResetTimerHandle;
	
	
// Montage Section
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
	bool CanToggleCombat() const;
	FORCEINLINE bool IsSprinting() const { return bSprinting; }

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
	/** 전투 활성화/비활성화 토글 */
	void ToggleCombat();
	void AutoToggleCombat();
	/* Attack */
	void Attack();
	void SpecialAttack();
	void HeavyAttack();
	/** LockOn 타겟팅 */
	void LockOnTarget();
	void LeftTarget();
	void RightTarget();

protected:
	/* 현재 상태에서 수행 가능한 일반공격 */
	FGameplayTag GetAttackPerform() const;

	/* 공격 가능 조건 체크 */
	bool CanPerformAttack(const FGameplayTag& AttackType) const;
	/* 공격 실행 */
	void DoAttack(const FGameplayTag& AttackType);
	/* 콤보 실행 */
	void ExecuteComboAttack(const FGameplayTag& AttackType);
	/* 콤보 리셋 */
	void ResetCombo();


// Combo AnimNotify Section
public:
	void EnableComboWindow();
	void DisableComboWindow();
	void AttackFinished(const float ComboResetDelay);
	
};


