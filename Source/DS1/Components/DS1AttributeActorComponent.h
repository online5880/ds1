#pragma once

#include "DS1Define.h"
#include "Components/ActorComponent.h"
#include "DS1AttributeActorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, EDS1AttributeType, float);
DECLARE_MULTICAST_DELEGATE(FOnDeath);

/**
 * DS1AttributeActorComponent
 * 이 컴포넌트는 캐릭터의 스태미나와 체력을 관리합니다.
 * 스태미나 재충전 기능과 스태미나 감소 기능을 포함합니다.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1AttributeActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** 스탯 변경 시 호출되는 델리게이트 */
	FDelegateOnAttributeChanged OnAttributeChanged;

	/** 캐릭터가 죽었을 때 호출되는 델리게이트 */
	FOnDeath OnDeath;

protected:
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float BaseStamina = 100.f; // 기본 스태미나

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.f; // 최대 스태미나

	UPROPERTY(Editanywhere, Category = "Stamina")
	float StaminaRegenRate = 0.2f; // 스태미나 재충전 속도

	UPROPERTY(EditAnywhere, Category = "Health")
	float BaseHealth = 100.f; // 기본 체력

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f; // 최대 체력

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
	FORCEINLINE float GetBaseHealth() const { return BaseHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	FORCEINLINE float GetStaminaRatio() const {return BaseStamina / MaxStamina; }
	FORCEINLINE float GetHealthRatio() const {return BaseHealth / MaxHealth; }

	/** 스태미나가 충분한지 확인 */
	bool CheckHasEnoughStamina(float StaminaCost) const;

	/** 스태미나를 감소시킴 */
	void DecreaseStamina(float StaminaCost);

	/** 스태미나 재충전 활성화/비활성화 */
	void ToggleStaminaRegeneration(bool bEnable, float StartDelay = 2.f);

	void BroadcastAttributeChanged(EDS1AttributeType InAttributeType) const; // 속성 변경 알림
	
	void TakeDamageAmount(float DamageAmount); // 체력 감소 함수

private:
	void RegenerateStaminaHandler();
};
