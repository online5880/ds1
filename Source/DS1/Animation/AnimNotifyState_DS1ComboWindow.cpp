// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_DS1ComboWindow.h"

#include "Character/DS1Character.h"

UAnimNotifyState_DS1ComboWindow::UAnimNotifyState_DS1ComboWindow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAnimNotifyState_DS1ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ADS1Character* Character = Cast<ADS1Character>(MeshComp->GetOwner()))
	{
		// 콤보 윈도우 활성화
		Character->EnableComboWindow();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast MeshComp owner to ADS1Character"));
	}
}

void UAnimNotifyState_DS1ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ADS1Character* Character = Cast<ADS1Character>(MeshComp->GetOwner()))
	{
		Character->DisableComboWindow();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast MeshComp owner to ADS1Character"));
	}
}
