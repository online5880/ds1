#pragma once
#include "DS1Define.h"
#include "Blueprint/UserWidget.h"
#include "DS1PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UDS1PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UDS1StatBarWidget* StaminaBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UDS1StatBarWidget* HealthBarWidget;

public:
	UDS1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeConstruct() override;

protected:
	void OnAttributeChanged(EDS1AttributeType AttributeType, float InValue);
};
