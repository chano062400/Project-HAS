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
					FHASAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
					Info.AbilityLevel = InAbilitySpec.Level;
					Info.StatusTag = HASASC->FindStatusTagByAbilitySpec(InAbilitySpec);
					Info.PlayerLevel = HASASC->FindPlayerLevelByAbilitySpec(InAbilitySpec);
					AbilityInfoDelegate.Broadcast(Info);
				}
			}
		);
	}
}
