#include "UI/WidgetController/HASWidgetController.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"

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
		if (!HASASC->bIsGivenStartAbilities) return;
		FForEachAbilitySignature ForEachAbilityDelegate;
		ForEachAbilityDelegate.BindLambda(
			[HASASC, this](const FGameplayAbilitySpec& InAbilitySpec)
			{
				const FGameplayTag AbilityTag = HASASC->FindAbilityTagByAbilitySpec(InAbilitySpec);
				FHASAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
				Info.AbilityLevel = InAbilitySpec.Level;
				Info.PlayerLevel = HASASC->FindPlayerLevelByAbilitySpec(InAbilitySpec);
				AbilityInfoDelegate.Broadcast(Info);
			}
		);

		HASASC->ForEachAbility(ForEachAbilityDelegate);
	}
}
