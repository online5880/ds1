
#include "UI/DS1StatBarWidget.h"
#include "Components/ProgressBar.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1StatBarWidget)

UDS1StatBarWidget::UDS1StatBarWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	
}

void UDS1StatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StatBar)
	{
		StatBar->SetFillColorAndOpacity(FillColorAndOpacity);
	}
}

void UDS1StatBarWidget::SetRatio(float Ratio) const
{
	if (StatBar)
	{
		StatBar->SetPercent(Ratio);
	}
}
