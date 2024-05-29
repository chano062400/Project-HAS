#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfoDataAsset.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AS);
	check(AttributeInfo);

	if (UHASAttributeSet* HASAS = Cast<UHASAttributeSet>(AS))
	{
		for (auto& Pair : HASAS->TagsToAttributes)
		{
			FHASAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
			Info.AttributeValue = Pair.Value.GetNumericValue(HASAS);
			StatChanged.Broadcast(Info.AttributeValue);
		}

		/*for (FHASAttributeInfo& Info : AttributeInfo->AttributeInformation)
		{
			Info.AttributeValue = HASAS->TagsToAttributes[Info.AttributeTag].GetNumericValue(HASAS);
			StatChanged.Broadcast(Info.AttributeValue);
		}*/
	}
}

void UAttributeMenuWidgetController::BindCallBacks()
{
	check(AS);
	check(AttributeInfo);

	if (UHASAttributeSet* HASAS = Cast<UHASAttributeSet>(AS))
	{
		for (auto& Pair : HASAS->TagsToAttributes)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
				[this](const FOnAttributeChangeData& NewValue)
				{
					StatChanged.Broadcast(NewValue.NewValue);
				}
			);
		}
	}
}
