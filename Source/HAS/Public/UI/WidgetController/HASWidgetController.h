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
	TObjectPtr<APlayerController> PC = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerState> PS = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AS = nullptr;
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

	virtual void BroadcastInitialValues();

	virtual void BindCallBacks();

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PC;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PS;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AS;	

};
