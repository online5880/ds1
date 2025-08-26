#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_UpdateMoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UBTTaskNode_UpdateMoveSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxWalkSpeed = 100.f;
	
};
