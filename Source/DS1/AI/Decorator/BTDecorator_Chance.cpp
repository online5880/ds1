#include "BTDecorator_Chance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_Chance)
bool UBTDecorator_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return ChanceRate > FMath::RandRange(1, 100);
}
