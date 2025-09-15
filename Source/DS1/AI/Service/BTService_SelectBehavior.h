#pragma once


#include "BehaviorTree/BTService.h"
#include "Character/DS1Enemy.h"
#include "BTService_SelectBehavior.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UBTService_SelectBehavior : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	ADS1Enemy* ControlledEnemy;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BehaviorKey;

	UPROPERTY(EditAnywhere)
	float AttackRangeDistance = 500.f;

public:
	UBTService_SelectBehavior();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	void SetBehaviorKey(UBlackboardComponent* InBlackboardComp, EDS1AIBehavior InBehavior) const;
	void UpdateBehavior(UBlackboardComponent* InBlackboardComp) const;
};
