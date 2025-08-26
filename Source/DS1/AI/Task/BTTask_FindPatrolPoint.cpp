#include "BTTask_FindPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DS1Enemy.h"
#include "Engine/TargetPoint.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_FindPatrolPoint)

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (ADS1Enemy* Character = Cast<ADS1Enemy>(ControlledPawn))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardLocation.SelectedKeyName, Character->GetPatrolPoint()->GetActorLocation());
		Character->IncrementPatrolPointIndex();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
