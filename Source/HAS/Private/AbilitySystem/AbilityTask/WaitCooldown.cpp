#include "AbilitySystem/AbilityTask/WaitCooldown.h"
#include "AbilitySystemComponent.h"

UWaitCooldown* UWaitCooldown::WaitForCooldown(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag)
{

	UWaitCooldown* WaitCooldown = NewObject<UWaitCooldown>();
	WaitCooldown->ASC = InASC;
	WaitCooldown->CooldownTag = InCooldownTag;

	if (!IsValid(InASC) || !InCooldownTag.IsValid())
	{
		WaitCooldown->EndTask();
		return nullptr;
	}

	// CooldownTag ���ŵ��� ��.
	InASC->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldown, &UWaitCooldown::CooldownTagChanged);

	// CooldownTag ������� ��.
	InASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldown, &UWaitCooldown::OnCooldownAppliedToSelf);

	return WaitCooldown;
}

void UWaitCooldown::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldown::OnCooldownAppliedToSelf(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		// �ش� CooldownTag�� ���� Effect�� Remaining�ð��� ������.
		FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		
		TArray<float> CooldownRemainings = InASC->GetActiveEffectsTimeRemaining(EffectQuery);
		if (CooldownRemainings.Num() > 0)
		{
			float MaxRemainingTime = CooldownRemainings[0];
			for (int i = 1; i < CooldownRemainings.Num(); i++)
			{
				MaxRemainingTime = FMath::Max<float>(MaxRemainingTime, CooldownRemainings[i]);
			}
			CooldownStart.Broadcast(MaxRemainingTime);
		}
	}

}

void UWaitCooldown::EndTask()
{
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	/** Call when the action is completely done, this makes the action free to delete, and will unregister it with the game instance */
	SetReadyToDestroy();

	/*Marks this object as Garbage.*/
	MarkAsGarbage();
}
