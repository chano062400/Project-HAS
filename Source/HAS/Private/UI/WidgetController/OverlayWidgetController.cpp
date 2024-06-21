#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Player/HASPlayerState.h"

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
	if (UHASAttributeSet* HASAttributeSet = Cast<UHASAttributeSet>(AS))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(HASAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this, HASAttributeSet](const FOnAttributeChangeData& Data)
			{
				MaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		ASC->GetGameplayAttributeValueChangeDelegate(HASAttributeSet->GetHealthAttribute()).AddLambda(
			[this, HASAttributeSet](const FOnAttributeChangeData& Data)
			{
				HealthChanged.Broadcast(Data.NewValue);
			}
		);

		ASC->GetGameplayAttributeValueChangeDelegate(HASAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this, HASAttributeSet](const FOnAttributeChangeData& Data)
			{
				MaxManaChanged.Broadcast(Data.NewValue);
			}
		);

		ASC->GetGameplayAttributeValueChangeDelegate(HASAttributeSet->GetManaAttribute()).AddLambda(
			[this, HASAttributeSet](const FOnAttributeChangeData& Data)
			{
				ManaChanged.Broadcast(Data.NewValue);
			}
		);
	}

	if (AHASPlayerState* HASPS = Cast<AHASPlayerState>(PS))
	{
		HASPS->PlayerLevelChangedDelegate.AddDynamic(this, &UOverlayWidgetController::PlayerXPChanged);
		HASPS->PlayerXPChangedDelegate.AddDynamic(this, &UOverlayWidgetController::PlayerLevelChanged);
	}
}

void UOverlayWidgetController::PlayerXPChanged(int32 NewXP)
{
	PlayerXPChangedDelegate.Broadcast(NewXP);
}

void UOverlayWidgetController::PlayerLevelChanged(int32 NewLevel)
{
	PlayerLevelChangedDelegate.Broadcast(NewLevel);
}
