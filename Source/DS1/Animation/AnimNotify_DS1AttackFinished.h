#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DS1AttackFinished.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UAnimNotify_DS1AttackFinished : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	float ComboResetDelay = 1.f; // 콤보 리셋 딜레이

public:
	UAnimNotify_DS1AttackFinished(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
