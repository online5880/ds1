
#include "DS1RotationComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1RotationComponent)

// Sets default values for this component's properties
UDS1RotationComponent::UDS1RotationComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}

void UDS1RotationComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDS1RotationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TargetActor || !bShouldRotate) return;

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
	GetOwner()->SetActorRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f));
}

