#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Player/HASPlayerState.h"
#include "AbilitySystem/Data/LevelXPInfo.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// 다운 캐스팅
	// WidgetController - UAttributeSet
	UHASAttributeSet* HASAttributeSet = CastChecked<UHASAttributeSet>(AS);
	{
		MaxHealthChanged.Broadcast(HASAttributeSet->GetMaxHealth());
		HealthChanged.Broadcast(HASAttributeSet->GetHealth());
		MaxManaChanged.Broadcast(HASAttributeSet->GetMaxMana());
		ManaChanged.Broadcast(HASAttributeSet->GetMana());
	} 
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
		}
	}

	if (AHASPlayerState* HASPS = Cast<AHASPlayerState>(PS))
	{
		HASPS->PlayerLevelChangedDelegate.AddLambda(
			[this](int32 NewLevel)
			{
				PlayerLevelChangedDelegate.Broadcast(static_cast<int32>(NewLevel));
			}
		);

		HASPS->PlayerXPChangedDelegate.AddLambda(
			[this,HASPS](int32 NewXP)
			{
				ULevelXPInfo* LevelXPInfo = HASPS->LevelXPInformation;
				int32 PlayerLevel = LevelXPInfo->FindLevelByXP(NewXP);
				int32 RequirementXP = LevelXPInfo->LevelXPInformations[PlayerLevel].RequirementXP;
				float Percent = (float) NewXP / (float) RequirementXP;

				PlayerXPPercentChangedDelegate.Broadcast(Percent);
			}
		);
	}
}
