#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Player/HASPlayerState.h"
#include "AbilitySystem/Data/LevelXPInfo.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	UHASAttributeSet* HASAttributeSet = CastChecked<UHASAttributeSet>(AS);
	{
		MaxHealthChanged.Broadcast(HASAttributeSet->GetMaxHealth());
		HealthChanged.Broadcast(HASAttributeSet->GetHealth());
		MaxManaChanged.Broadcast(HASAttributeSet->GetMaxMana());
		ManaChanged.Broadcast(HASAttributeSet->GetMana());
	} 

	if (AHASPlayerState* HASPS = Cast<AHASPlayerState>(PS))
	{
		PlayerLevelChangedDelegate.Broadcast(HASPS->GetLevel(), false);
		
		ULevelXPInfo* LevelXPInfo = HASPS->LevelXPInformation;
		int32 CurXP = HASPS->GetXP();
		int32 PlayerLevel = LevelXPInfo->FindLevelByXP(CurXP);
		int32 RequirementXP = LevelXPInfo->LevelXPInformations[PlayerLevel].RequirementXP;
		float Percent = (float)CurXP / (float)RequirementXP;

		PlayerXPPercentChangedDelegate.Broadcast(Percent);
	}

	BroadcastInitialAbilityInfo();
}

void UOverlayWidgetController::BindCallBacks()
{
	if (UHASAttributeSet* HASAS = Cast<UHASAttributeSet>(AS))
	{
		if (UHASAbilitySystemComponent* HASASC = Cast<UHASAbilitySystemComponent>(ASC))
		{
			HASASC->GetGameplayAttributeValueChangeDelegate(HASAS->GetMaxHealthAttribute()).AddLambda(
				[this, HASAS](const FOnAttributeChangeData& Data)
				{
					MaxHealthChanged.Broadcast(Data.NewValue);
				}
			);

			HASASC->GetGameplayAttributeValueChangeDelegate(HASAS->GetHealthAttribute()).AddLambda(
				[this, HASAS](const FOnAttributeChangeData& Data)
				{
					HealthChanged.Broadcast(Data.NewValue);
				}
			);

			HASASC->GetGameplayAttributeValueChangeDelegate(HASAS->GetMaxManaAttribute()).AddLambda(
				[this, HASAS](const FOnAttributeChangeData& Data)
				{
					MaxManaChanged.Broadcast(Data.NewValue);
				}
			);

			HASASC->GetGameplayAttributeValueChangeDelegate(HASAS->GetManaAttribute()).AddLambda(
				[this, HASAS](const FOnAttributeChangeData& Data)
				{
					ManaChanged.Broadcast(Data.NewValue);
				}
			);

			HASASC->AbilityUpdateDelegate.AddLambda(
				[this, HASASC](FGameplayAbilitySpec& InAbilitySpec)
				{
					const FGameplayTag AbilityTag = HASASC->FindAbilityTagByAbilitySpec(InAbilitySpec);
					FHASAbilityInfo Info = UHASAbilitySystemBlueprintLibrary::FindAbilityInfoByTag(HASASC->GetAvatarActor(), AbilityTag);
					Info.AbilityLevel = InAbilitySpec.Level;
					Info.StatusTag = HASASC->FindStatusTagByAbilitySpec(InAbilitySpec);
					Info.PlayerLevel = HASASC->FindPlayerLevelByAbilitySpec(InAbilitySpec);
					AbilityInfoDelegate.Broadcast(Info);
				}
			);

			if (HASASC->bIsGivenStartAbilities) BroadcastInitialAbilityInfo();
			else HASASC->StartAbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastInitialAbilityInfo);
		}
	}

	if (AHASPlayerState* HASPS = Cast<AHASPlayerState>(PS))
	{
		HASPS->PlayerXPChangedDelegate.AddLambda(
			[this, HASPS](int32 NewXP)
			{
				ULevelXPInfo* LevelXPInfo = HASPS->LevelXPInformation;
				int32 PlayerLevel = LevelXPInfo->FindLevelByXP(NewXP);
				int32 RequirementXP = LevelXPInfo->LevelXPInformations[PlayerLevel].RequirementXP;
				float Percent = (float)NewXP / (float)RequirementXP;

				PlayerXPPercentChangedDelegate.Broadcast(Percent);
			}
		);

		HASPS->PlayerLevelChangedDelegate.AddLambda(
			[this](int32 NewLevel)
			{
				PlayerLevelChangedDelegate.Broadcast(NewLevel, true);
			}
		);
	}
}
