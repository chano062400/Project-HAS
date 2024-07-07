#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfoDataAsset.h"
#include "Player/HASPlayerState.h"
#include "Interfaces/HASPlayerInterface.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AS);
	check(AttributeInfo);

	if (UHASAttributeSet* HASAS = Cast<UHASAttributeSet>(AS))
	{
 		for (auto& Pair : HASAS->TagToAttribute)
		{
			FHASAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
			Info.AttributeValue = Pair.Value.GetNumericValue(HASAS);
			AttributeInfoDelegate.Broadcast(Info);
		}
	}

	if (AHASPlayerState* HASPS = Cast<AHASPlayerState>(PS))
	{
		AttributePointChangedDelegate.Broadcast(HASPS->GetAttributePoint());
	}
}

void UAttributeMenuWidgetController::BindCallBacks()
{
	check(AS);
	check(AttributeInfo);

	if (UHASAttributeSet* HASAS = Cast<UHASAttributeSet>(AS))
	{
		for (auto& Pair : HASAS->TagToAttribute)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
				[this, Pair](const FOnAttributeChangeData& NewValue)
				{
					StatChangedDelegate.Broadcast(Pair.Key, NewValue.NewValue);
				}
			);
		}
	}

	if (AHASPlayerState* HASPS = Cast<AHASPlayerState>(PS))
	{
		HASPS->PlayerAttributePointChangedDelegate.AddLambda(
			[this](int32 NewAttributePoint)
			{
				AttributePointChangedDelegate.Broadcast(NewAttributePoint);
			}
		);
	}
}

void UAttributeMenuWidgetController::UseAttributePointPressed(const FGameplayTag& AttributeTag)
{
	if (UHASAttributeSet* HASAS = Cast<UHASAttributeSet>(AS))
	{
		if (UHASAbilitySystemComponent* HASASC = Cast<UHASAbilitySystemComponent>(ASC))
		{
			if (HASASC->GetAvatarActor()->Implements<UHASPlayerInterface>())
			{
				IHASPlayerInterface* PlayerInterface = Cast<IHASPlayerInterface>(HASASC->GetAvatarActor());

				for (TTuple<FGameplayTag, FGameplayAttribute> pair : HASAS->TagToAttribute)
				{
					if (pair.Key.MatchesTagExact(AttributeTag))
					{
						// +버튼을 누른 Attribute를 1증가하고, Attribute Point는 1감소.
						HASASC->ServerUpdateAttribute(pair.Value, EGameplayModOp::Additive, 1.f);

						PlayerInterface->SetAttributePoint(PlayerInterface->GetAttributePoint() - 1);
					}
				}
			}
		}
	}
}
