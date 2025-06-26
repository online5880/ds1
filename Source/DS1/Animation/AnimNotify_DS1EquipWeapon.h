#pragma once


#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DS1EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UAnimNotify_DS1EquipWeapon : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	FGameplayTag MontageActionTag;

	public:
	UAnimNotify_DS1EquipWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
