// Fill out your copyright notice in the Description page of Project Settings.


#include "DS1CombatComponent.h"

UCombatComponent::UCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::SetWeapon(ADS1Weapon* NewWeapon)
{
	MainWeapon = NewWeapon;
}

