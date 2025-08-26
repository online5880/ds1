#include "BTTaskNode_UpdateMoveSpeed.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTaskNode_UpdateMoveSpeed)

EBTNodeResult::Type UBTTaskNode_UpdateMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (UCharacterMovementComponent* MovementComp = ControlledPawn->GetComponentByClass<UCharacterMovementComponent>())
	{
		MovementComp->MaxWalkSpeed = MaxWalkSpeed;
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
