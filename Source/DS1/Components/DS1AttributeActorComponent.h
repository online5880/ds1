// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS1Define.h"
#include "Components/ActorComponent.h"
#include "DS1AttributeActorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, EDS1AttributeType, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1AttributeActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** 스탯 변경 시 호출되는 델리게이트 */
	FDelegateOnAttributeChanged OnAttributeChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float BaseStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(Editanywhere, Category = "Stamina")
	float StaminaRegenRate = 0.2f; // 스태미나 재충전 속도

	/** 스태미나 재충전 타이머 핸들 */
	FTimerHandle StaminaRegenTimerHandle;

public:	

	UDS1AttributeActorComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	FORCEINLINE float GetStaminaRatio() const {return BaseStamina / MaxStamina; }

	/** 스태미나가 충분한지 확인 */
	bool CheckHasEnoughStamina(float StaminaCost) const;

	/** 스태미나를 감소시킴 */
	void DecreaseStamina(float StaminaCost);

	/** 스태미나 재충전 활성화/비활성화 */
	void ToggleStaminaRegeneration(bool bEnable, float StartDelay = 2.f);

	void BroadcastAttributeChanged(EDS1AttributeType InAttributeType) const;

private:
	void RegenerateStaminaHandler();
};
