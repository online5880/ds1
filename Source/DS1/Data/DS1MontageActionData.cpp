#include "DS1MontageActionData.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1MontageActionData)

UAnimMontage* UDS1MontageActionData::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index)
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FDS1MontageGroup& MontageGroup = MontageGroupMap[GroupTag];
		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}
	}
	return nullptr;
}
