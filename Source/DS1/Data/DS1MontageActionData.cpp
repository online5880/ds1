#include "DS1MontageActionData.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1MontageActionData)

UAnimMontage* UDS1MontageActionData::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index) const
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

UAnimMontage* UDS1MontageActionData::GetRandomMontageForTag(const FGameplayTag& GroupTag) const
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FDS1MontageGroup& MontageGroup = MontageGroupMap[GroupTag];

		// 랜덤 Index
		const int32 RandomIndex = FMath::RandRange(0, MontageGroup.Animations.Num() -1);

		return GetMontageForTag(GroupTag, RandomIndex);
	}
	return nullptr;
}
