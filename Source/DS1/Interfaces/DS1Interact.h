﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DS1Interact.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDS1Interact : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DS1_API IDS1Interact
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(AActor* Interactor) = 0;
};
