#pragma once

#include "Runtime/AIModule/Classes/AIController.h"
#include "DS1EnemyAIController.generated.h"

class ADS1Enemy;

UCLASS()
class DS1_API ADS1EnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTreeAsset;

public:
	ADS1EnemyAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	FTimerHandle TimerHandle;

	UPROPERTY()
	ADS1Enemy* ControlledEnemy;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	void UpdateTarget() const;
	void SetTarget(AActor* NewTarget) const;
	
};
