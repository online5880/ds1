#include "BTTask_PerformAttack.h"

#include "AIController.h"
#include "Components/DS1StateComponent.h"
#include "Interfaces/DS1CombatInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_PerformAttack)

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (IDS1CombatInterface* CombatInterface = Cast<IDS1CombatInterface>(ControlledPawn))
	{
		FOnMontageEnded MontageEndedDelegate;

		MontageEndedDelegate.BindLambda([this, &OwnerComp, ControlledPawn](UAnimMontage* Montage, bool bInterrupted)
		{
			UE_LOG(LogTemp, Error, TEXT("Execute MontageEndedDelegate"));

			if (::IsValid(&OwnerComp) == false)
			{
				return;
			}

			if (UDS1StateComponent* StateComponent = ControlledPawn->GetComponentByClass<UDS1StateComponent>())
			{
				StateComponent->ClearState();
			}
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

		CombatInterface->PerformAttack(AttackTypeTag, MontageEndedDelegate);

		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}
