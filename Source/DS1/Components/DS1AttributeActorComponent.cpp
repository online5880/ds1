#include "Components/DS1AttributeActorComponent.h"


UDS1AttributeActorComponent::UDS1AttributeActorComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UDS1AttributeActorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UDS1AttributeActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UDS1AttributeActorComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void UDS1AttributeActorComponent::DecreaseStamina(float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);
}

void UDS1AttributeActorComponent::ToggleStaminaRegeneration(bool bEnable, float StartDelay)
{
	if (bEnable)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UDS1AttributeActorComponent::RegenerateStaminaHandler, 0.1f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void UDS1AttributeActorComponent::RegenerateStaminaHandler()
{
	BaseStamina = FMath::Clamp(BaseStamina + StaminaRegenRate, 0.f, MaxStamina);

	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false); // 스태미나가 최대치에 도달하면 재충전 중지
	}
}

