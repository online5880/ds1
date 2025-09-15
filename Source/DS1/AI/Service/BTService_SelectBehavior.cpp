#include "BTService_SelectBehavior.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTService_SelectBehavior)

UBTService_SelectBehavior::UBTService_SelectBehavior()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_SelectBehavior::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn) return;

	ControlledEnemy = Cast<ADS1Enemy>(ControlledPawn);
	
}

void UBTService_SelectBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UpdateBehavior(OwnerComp.GetBlackboardComponent());
}

void UBTService_SelectBehavior::SetBehaviorKey(UBlackboardComponent* InBlackboardComp, EDS1AIBehavior InBehavior) const
{
	InBlackboardComp->SetValueAsEnum(BehaviorKey.SelectedKeyName, static_cast<uint8>(InBehavior));
}

void UBTService_SelectBehavior::UpdateBehavior(UBlackboardComponent* InBlackboardComp) const
{
	check(InBlackboardComp)
	check(ControlledEnemy)

	AActor* TargetActor = Cast<AActor>(InBlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (IsValid(TargetActor))		// 타겟이 있으면
	{
		const float Distance = TargetActor->GetDistanceTo(ControlledEnemy);

		// 공격범위 안쪽이면
		if (Distance <= AttackRangeDistance)
		{
			SetBehaviorKey(InBlackboardComp, EDS1AIBehavior::MeleeAttack);	// 공격
		}
		else
		{
			SetBehaviorKey(InBlackboardComp, EDS1AIBehavior::Approach);		// 추적
		}
	}
	else							// 타겟이 없으면
	{
		// Patrol Point 가 있으면
		if (ControlledEnemy->GetPatrolPoint() != nullptr)
		{
			SetBehaviorKey(InBlackboardComp, EDS1AIBehavior::Patrol);	 // 순찰 
		}
		else
		{
			SetBehaviorKey(InBlackboardComp, EDS1AIBehavior::Idle);		// 기본
		}
	}
}
