#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HASWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* InPC, APlayerState* InPS, UAbilitySystemComponent* InASC, UAttributeSet* InAS)
		: PC(InPC), PS(InPS), ASC(InASC), AS(InAS) {}

	UPROPERTY()
	APlayerController* PC = nullptr;

	UPROPERTY()
	APlayerState* PS = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* ASC = nullptr;

	UPROPERTY()
	UAttributeSet* AS = nullptr;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HAS_API UHASWidgetController : public UObject
{
	GENERATED_BODY()

public:

	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	APlayerController* PC;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	APlayerState* PS;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UAbilitySystemComponent* ASC;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UAttributeSet* AS;	

};
