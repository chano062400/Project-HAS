#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/Ability/HASGameplayAbility.h"
#include "HASGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "HAS/HASGameModeBase.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/HASCombatInterface.h"
#include "Interfaces/HASPlayerInterface.h"

void UHASAbilitySystemComponent::AbilityActorInfoSet()
{
	/* Called on server whenever a GE is applied to self. This includes instant and duration based GEs. 
	FOnGameplayEffectAppliedDelegate OnGameplayEffectAppliedDelegateToSelf; */
	/*OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UHASAbilitySystemComponent::ClientEffectApplied);*/
}


// StartAbility�� ����� ������ Broadcast.
void UHASAbilitySystemComponent::AddStartAbilitiesByInputTag(TArray<TSubclassOf<UGameplayAbility>> StartAbilities)
{
	for (auto Ability : StartAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability, 1.f);

		if (UHASGameplayAbility* HASAbility = Cast<UHASGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.SourceObject = GetAvatarActor();

			//AbilitySpec�� InputTag�� �߰�.
			AbilitySpec.DynamicAbilityTags.AddTag(HASAbility->InputTag);

			AbilitySpec.DynamicAbilityTags.AddTag(FHASGameplayTags::Get().Status_Equipped);

			GiveAbility(AbilitySpec);
			AbilityUpdateDelegate.Broadcast(AbilitySpec, true);
		}
	}
}

void UHASAbilitySystemComponent::AddHitReactAbility(TSubclassOf<UGameplayAbility> HitReactAbility)
{
	FGameplayAbilitySpec AbilitySpec(HitReactAbility, 1.f);
	GiveAbility(AbilitySpec);
}

void UHASAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// ��� Ability�� Ž���ϴ� ���� Ability�� �������� ���ϵ��� Lock
	FScopedAbilityListLock ScopeAbilityListLcok(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//Spec�� ���� GA�� �Է��� �������ִ� �Լ�
			AbilitySpecInputPressed(AbilitySpec);

			if (AbilitySpec.IsActive())
			{
				// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Pressed")));
		}
	}
}

void UHASAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// ��� Ability�� Ž���ϴ� ���� Ability�� �������� ���ϵ��� Lock
	FScopedAbilityListLock ScopeAbilityListLcok(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//Spec�� ���� GA�� �Է��� �������ִ� �Լ�
			AbilitySpecInputPressed(AbilitySpec);

			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UHASAbilitySystemComponent::ApplyDefaultAttributesByClass(ECharacterClass CharacterClass, int32 Level)
{
	if (AHASGameModeBase* HASGameMode = Cast<AHASGameModeBase>(UGameplayStatics::GetGameMode(GetAvatarActor())))
	{
		FClassDefaultInfo Info = HASGameMode->ClassInformation->GetCharacterClassInfo(CharacterClass);

		FGameplayEffectContextHandle PrimaryEffectContextHandle = MakeEffectContext();
		PrimaryEffectContextHandle.AddSourceObject(GetAvatarActor());
		FGameplayEffectSpecHandle PrimaryAttributeEffectSpec = MakeOutgoingSpec(Info.PrimaryAttribute, Level, PrimaryEffectContextHandle);
		ApplyGameplayEffectSpecToSelf(*PrimaryAttributeEffectSpec.Data.Get());

		FGameplayEffectContextHandle SecondaryEffectContextHandle = MakeEffectContext();
		SecondaryEffectContextHandle.AddSourceObject(GetAvatarActor());
		FGameplayEffectSpecHandle SecondaryAttributeEffectSpec = MakeOutgoingSpec(HASGameMode->ClassInformation->SecondaryAttribute, Level, SecondaryEffectContextHandle);
		ApplyGameplayEffectSpecToSelf(*SecondaryAttributeEffectSpec.Data.Get());

		FGameplayEffectContextHandle VitalEffectContextHandle = MakeEffectContext();
		VitalEffectContextHandle.AddSourceObject(GetAvatarActor());
		FGameplayEffectSpecHandle VitalAttributeEffectSpec = MakeOutgoingSpec(HASGameMode->ClassInformation->VitalAttribute, Level, VitalEffectContextHandle);
		ApplyGameplayEffectSpecToSelf(*VitalAttributeEffectSpec.Data.Get());
	}
}

void UHASAbilitySystemComponent::AddDefaultAbilitiesByClass(ECharacterClass CharacterClass, int32 Level)
{
	if(AHASGameModeBase* HASGameMode = Cast<AHASGameModeBase>(UGameplayStatics::GetGameMode(GetAvatarActor())))
	{
		FClassDefaultInfo Info = HASGameMode->ClassInformation->GetCharacterClassInfo(CharacterClass);
		
		for (TSubclassOf<UGameplayAbility> Ability : Info.DefaultAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(Ability, Level);

			GiveAbility(AbilitySpec);
		}
	}
}

void UHASAbilitySystemComponent::AddCommonAbilities()
{
	if (AHASGameModeBase* HASGameMode = Cast<AHASGameModeBase>(UGameplayStatics::GetGameMode(GetAvatarActor())))
	{
		for (TSubclassOf<UGameplayAbility> Ability : HASGameMode->ClassInformation->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(Ability, 1.f);

			GiveAbility(AbilitySpec);
		}
	}
}

void UHASAbilitySystemComponent::RemoveAllDebuffEffect()
{
	FGameplayTagContainer TagContainer;
	// Debuff Tag�� ��� �߰�.
	TagContainer.AddTag(FHASGameplayTags::Get().Debuff_Burn);
	RemoveActiveEffectsWithGrantedTags(TagContainer);
}

void UHASAbilitySystemComponent::AbilityLevelUp(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByTag(AbilityTag))
	{
		if (IHASPlayerInterface* PlayerInterface = Cast<IHASPlayerInterface>(GetAvatarActor()))
		{
			int32 CurSpellPoint = PlayerInterface->GetSpellPoint();
			PlayerInterface->SetSpellPoint(CurSpellPoint - 1);
		}

		FGameplayTag StatusTag = FindStatusTagByAbilitySpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(FHASGameplayTags::Get().Status_UnLocked))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(FHASGameplayTags::Get().Status_UnLocked);
			AbilitySpec->DynamicAbilityTags.AddTag(FHASGameplayTags::Get().Status_UnEquipped);
		}

		AbilitySpec->Level = FMath::Clamp(AbilitySpec->Level + 1, 0, 5);

		AbilityUpdateDelegate.Broadcast(*AbilitySpec, false);
	}
}

FGameplayAbilitySpec* UHASAbilitySystemComponent::FindAbilitySpecByTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock AbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTagExact(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}

	return nullptr;
}

FGameplayTag UHASAbilitySystemComponent::FindAbilityTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag AbilityTag : AbilitySpec.Ability->AbilityTags)
		{
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
			{
				return AbilityTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UHASAbilitySystemComponent::FindStatusTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

int32 UHASAbilitySystemComponent::FindPlayerLevelByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.SourceObject->Implements<UHASCombatInterface>())
	{
		return IHASCombatInterface::Execute_GetLevel(AbilitySpec.SourceObject.Get());
	}
	return 0;
}

void UHASAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// ��� Ability�� Ž���ϴ� ���� Ability�� �������� ���ϵ��� Lock
	FScopedAbilityListLock ScopeAbilityListLcok(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//Spec�� ���� GA�� �Է��� �������ִ� �Լ�
			AbilitySpecInputReleased(AbilitySpec);

			if (AbilitySpec.IsActive())
			{
				// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
				
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Released")));
			}
		}
	}
}

void UHASAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{

}
