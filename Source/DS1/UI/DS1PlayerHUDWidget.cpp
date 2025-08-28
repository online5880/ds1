#include "UI/DS1PlayerHUDWidget.h"
#include "DS1StatBarWidget.h"
#include "Components/DS1AttributeActorComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1PlayerHUDWidget)

UDS1PlayerHUDWidget::UDS1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDS1PlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (UDS1AttributeActorComponent* Attribute = OwningPawn->GetComponentByClass<UDS1AttributeActorComponent>())
		{
			Attribute->OnAttributeChanged.AddUObject(this, &UDS1PlayerHUDWidget::OnAttributeChanged);
			Attribute->BroadcastAttributeChanged(EDS1AttributeType::Stamina);
			Attribute->BroadcastAttributeChanged(EDS1AttributeType::Health);
		}
	}
}

void UDS1PlayerHUDWidget::OnAttributeChanged(EDS1AttributeType AttributeType, float InValue)
{
	switch (AttributeType)
	{
	case EDS1AttributeType::Stamina:
		StaminaBarWidget->SetRatio(InValue);
		break;
		
	case EDS1AttributeType::Health:
		HealthBarWidget->SetRatio(InValue);
		break;
	}
}
