#include "UI/WidgetController/HASWidgetController.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"

void UHASWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PC = WCParams.PC;
	PS = WCParams.PS;
	ASC = WCParams.ASC;
	AS = WCParams.AS;
}

void UHASWidgetController::BroadcastInitialValues()
{
}

void UHASWidgetController::BindCallBacks()
{
}

void UHASWidgetController::BroadcastInitialAbilityInfo()
{
	// StartAbilities Info Broadcast
	if (UHASAbilitySystemComponent* HASASC = Cast<UHASAbilitySystemComponent>(ASC))
	{
		HASASC->AbilityUpdateDelegate.AddLambda(
			[this, HASASC](FGameplayAbilitySpec& InAbilitySpec, bool bIsStartAbility)
			{
				if (bIsStartAbility)
				{
					const FGameplayTag AbilityTag = HASASC->FindAbilityTagByAbilitySpec(InAbilitySpec);
					AbilityInfoDelegate.Broadcast(AbilityInfo->FindAbilityInfoByTag(AbilityTag));
				}
			}
		);
	}
}
