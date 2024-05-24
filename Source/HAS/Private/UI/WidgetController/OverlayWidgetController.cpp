#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/HASAttributeSet.h"

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
				HealthChanged.Broadcast(Data.NewValue);
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
				ManaChanged.Broadcast(Data.NewValue);
			}
		);

		ASC->GetGameplayAttributeValueChangeDelegate(HASAttributeSet->GetManaAttribute()).AddLambda(
			[this, HASAttributeSet](const FOnAttributeChangeData& Data)
			{
				ManaChanged.Broadcast(Data.NewValue);
			}
		);

	}
}
