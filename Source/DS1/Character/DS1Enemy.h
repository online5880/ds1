#pragma once

#include "GameFramework/Character.h"
#include "Interfaces/DS1CombatInterface.h"
#include "Interfaces/DS1Targeting.h"
#include "DS1Enemy.generated.h"

class ADS1Weapon;
class ATargetPoint;
/**
 *  적 캐릭터 클래스
 */
UCLASS()
class DS1_API ADS1Enemy : public ACharacter, public IDS1Targeting, public IDS1CombatInterface
{
	GENERATED_BODY()

protected:
	// 타겟팅 충돌 체크용
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	class USphereComponent* TargetingSphereComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UDS1AttributeActorComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	class UDS1StateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere)
	class UDS1CombatComponent* CombatComponent;

	// Lock On UI Widget
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	class UWidgetComponent* LockOnWidgetComponent;

// Effect Section
protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<ADS1Weapon> DefaultWeaponClass;
	
	
public:
	ADS1Enemy();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void OnDeath();

protected:
	void ImpactEffect(const FVector& Location);
	void HitReact(const AActor* Attacker);

public:
	// IDS1Targeting Interface 구현
	// 타겟팅시 처리할 로직
	virtual void OnTargeted(bool bTargeted) override;
	// 타겟팅 가능한지 체크
	virtual bool CanBeTargeted() override;

	// IDS1CombatInterface 구현
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) override;

public:
	FORCEINLINE ATargetPoint* GetPatrolPoint()
	{
		return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr;
	}
	FORCEINLINE void IncrementPatrolPointIndex()
	{
		PatrolIndex = (PatrolIndex + 1) % (PatrolPoints.Num());
	}
};
