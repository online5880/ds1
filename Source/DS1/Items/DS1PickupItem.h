// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DS1Interact.h"
#include "DS1PickupItem.generated.h"

UCLASS()
class DS1_API ADS1PickupItem : public AActor, public IDS1Interact
{
	GENERATED_BODY()
	
public:	
	ADS1PickupItem();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* Mesh;

};
