#pragma once

#include "UObject/Interface.h"
#include "DS1Targeting.generated.h"

UINTERFACE()
class UDS1Targeting : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DS1_API IDS1Targeting
{
	GENERATED_BODY()

public:
	virtual void OnTargeted(bool bTargeted) = 0;
	virtual bool CanBeTargeted() = 0;
};
