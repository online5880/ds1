#include "AnimNotifyState_DS1RotateTo.h"

#include "Components/DS1RotationComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimNotifyState_DS1RotateTo)

void UAnimNotifyState_DS1RotateTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UDS1RotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<UDS1RotationComponent>())
	{
		RotationComponent->ToggleShouldRotate(true);
	}
}

void UAnimNotifyState_DS1RotateTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UDS1RotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<UDS1RotationComponent>())
	{
		RotationComponent->ToggleShouldRotate(false);
	}
}
