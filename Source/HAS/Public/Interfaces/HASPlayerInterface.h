#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HASPlayerInterface.generated.h"

class UAttributeSet;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHASPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HAS_API IHASPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetXP(int32 NewXP) = 0;
	virtual void SetLevel(int32 NewLevel) = 0;
	virtual int32 GetXP() = 0;
	virtual int32 GetAttributePoint() = 0;
	virtual void SetAttributePoint(int32 NewAttributePoint) = 0;
	virtual UAttributeSet* GetAttributeSet() = 0;
};
