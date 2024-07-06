#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "Player/HASPlayerState.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastInitialAbilityInfo();
	SpellPointChangedDelegate.Broadcast(0);
}

void USpellMenuWidgetController::BindCallBacks()
{
	if (UHASAbilitySystemComponent* HASASC = Cast<UHASAbilitySystemComponent>(ASC))
	{
		HASASC->AbilityUpdateDelegate.AddLambda(
			[this, HASASC](FGameplayAbilitySpec& InAbilitySpec)
			{
					const FGameplayTag AbilityTag = HASASC->FindAbilityTagByAbilitySpec(InAbilitySpec);
					FHASAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
					Info.AbilityLevel = InAbilitySpec.Level;
					Info.StatusTag = HASASC->FindStatusTagByAbilitySpec(InAbilitySpec);
					Info.PlayerLevel = HASASC->FindPlayerLevelByAbilitySpec(InAbilitySpec);
					//Info.Ability = Cast<UHASGameplayDamageAbility>(InAbilitySpec.Ability);
					AbilityInfoDelegate.Broadcast(Info);
			}
		);

		HASASC->StartAbilitiesGivenDelegate.AddUObject(this, &USpellMenuWidgetController::BroadcastInitialAbilityInfo);
	}

	if (AHASPlayerState* HASPS = Cast<AHASPlayerState>(PS))
	{
		HASPS->PlayerSpellPointChangedDelegate.AddLambda(
			[this](int32 NewSpellPoint)
			{
				SpellPointChangedDelegate.Broadcast(NewSpellPoint);
			}
		);
	}
}

void USpellMenuWidgetController::SpellBoxPressed(const FGameplayTag& AbilityTag)
{
	if (AHASPlayerState* HASPS = Cast<AHASPlayerState>(PS))
	{
		int32 CurSpellPoint = HASPS->GetSpellPoint();
		if (CurSpellPoint <= 0) return;

		if (UHASAbilitySystemComponent* HASASC = Cast<UHASAbilitySystemComponent>(ASC))
		{
			HASASC->ServerUpdateAbility(AbilityTag);
		}
	}
}
