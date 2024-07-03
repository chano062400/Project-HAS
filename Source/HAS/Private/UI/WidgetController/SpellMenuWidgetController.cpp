#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastInitialAbilityInfo();
}

void USpellMenuWidgetController::BindCallBacks()
{
	if (UHASAbilitySystemComponent* HASASC = Cast<UHASAbilitySystemComponent>(ASC))
	{
		HASASC->AbilityUpdateDelegate.AddLambda(
			[this, HASASC](FGameplayAbilitySpec& InAbilitySpec, bool bIsStartAbility)
			{
				if (!bIsStartAbility)
				{
					const FGameplayTag AbilityTag = HASASC->FindAbilityTagByAbilitySpec(InAbilitySpec);
					AbilityInfoDelegate.Broadcast(AbilityInfo->FindAbilityInfoByTag(AbilityTag));
				}
			}
		);
	}
}
