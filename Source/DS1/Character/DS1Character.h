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
	UInputAction* SprintAction;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly ,Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UDS1AttributeActorComponent* AttributeComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float SprintSpeed = 750.f; // 스프린트 속도

	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float NormalSpeed = 500.f; // 일반 속도
	
public:
	ADS1Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	bool IsMoving() const;
	
	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
	
	void Sprinting();
	
	void StopSprint();
};


