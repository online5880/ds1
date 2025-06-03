// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DS1StateComponent.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UDS1StateComponent::UDS1StateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDS1StateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDS1StateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDS1StateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = FName("MovementInputEnableAction");
		LatentAction.UUID = 0; // 고유 ID
		LatentAction.Linkage = 0; // 링크 번호

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(),Duration, LatentAction);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}

void UDS1StateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
}

bool UDS1StateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	// 현재 상태가 TagsToCheck에 포함되어 있는지 확인
	return TagsToCheck.HasTagExact(CurrentState);
}

