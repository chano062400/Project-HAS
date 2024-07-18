#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/Ability/HASGameplayAbility.h"
#include "HASGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "HAS/HASGameModeBase.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/HASCombatInterface.h"
#include "Interfaces/HASPlayerInterface.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"

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
		}
	}

	bIsGivenStartAbilities = true;

	StartAbilitiesGivenDelegate.Broadcast();
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
	TagContainer.AddTag(FHASGameplayTags::Get().Debuff_ElectricShock);
	TagContainer.AddTag(FHASGameplayTags::Get().Debuff_Freeze);
	RemoveActiveEffectsWithGrantedTags(TagContainer);
}

void UHASAbilitySystemComponent::AbilityLevelUp(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByAbilityTag(AbilityTag))
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

		AbilityUpdateDelegate.Broadcast(*AbilitySpec);

		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UHASAbilitySystemComponent::ForEachAbility(const FForEachAbilitySignature& Delegate)
{
	FScopedAbilityListLock AbilityListLock(*this);

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (Delegate.IsBound())
		{
			Delegate.Execute(AbilitySpec);
		}
	}
}

void UHASAbilitySystemComponent::ServerUpdateAbilityStatus_Implementation(int32 Level)
{
	if (UAbilityInfo* AbilityInfo = UHASAbilitySystemBlueprintLibrary::GetAbilityInfo(GetAvatarActor()))
	{
		for (const FHASAbilityInfo& Info : AbilityInfo->AbilityInformation)
		{
			// �̹� ��ϵ� Ability�� �ǳʶ�.(�̹� UnLocked���� �̻��̱� ����.)
			if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByAbilityTag(Info.AbilityTag))
			{
				continue;
			}
			else if (Info.StatusTag.MatchesTagExact(FHASGameplayTags::Get().Status_Locked))
			{
				if (Info.RequirementLevel > Level) continue;

				// �䱸 ������ �����ϸ� ��� �� �ִ� ���� UnLocked Status�� �ǰ� GiveAbility.
				FGameplayAbilitySpec NewAbilitySpec(Info.Ability.Get(), 0.f);

				NewAbilitySpec.DynamicAbilityTags.AddTag(FHASGameplayTags::Get().Status_UnLocked);
				NewAbilitySpec.SourceObject = GetAvatarActor();
				GiveAbility(NewAbilitySpec);

				ClientUpdateAbilityStatus(NewAbilitySpec);

				MarkAbilitySpecDirty(NewAbilitySpec);
			}
		}
	}
}

void UHASAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(FGameplayAbilitySpec AbilitySpec)
{
	AbilityUpdateDelegate.Broadcast(AbilitySpec);
}

void UHASAbilitySystemComponent::ServerUpgradeAbility_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByAbilityTag(AbilityTag))
	{
		if (IHASPlayerInterface* PlayerInterface = Cast<IHASPlayerInterface>(GetAvatarActor()))
		{
			int32 CurSpellPoint = PlayerInterface->GetSpellPoint();
			PlayerInterface->SetSpellPoint(CurSpellPoint - 1);
		}

		FGameplayTag StatusTag = FindStatusTagByAbilitySpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(FHASGameplayTags::Get().Status_UnLocked))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(FindStatusTagByAbilitySpec(*AbilitySpec));
			AbilitySpec->DynamicAbilityTags.AddTag(FHASGameplayTags::Get().Status_UnEquipped);
		}

		AbilitySpec->Level = FMath::Clamp(AbilitySpec->Level + 1, 0, 5);

		ClientUpgradeAbility(*AbilitySpec);

		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UHASAbilitySystemComponent::ClientUpgradeAbility_Implementation(FGameplayAbilitySpec AbilitySpec)
{
	AbilityUpdateDelegate.Broadcast(AbilitySpec);
}

void UHASAbilitySystemComponent::ServerUpdateAttribute_Implementation(const FGameplayAttribute& Attribute, EGameplayModOp::Type ModOp, float Value)
{
	ApplyModToAttribute(Attribute, ModOp, Value);
}

void UHASAbilitySystemComponent::ServerUpdateAbilityInput_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByAbilityTag(AbilityTag))
	{
		const FGameplayTag CurInputTag = FindInputTagByAbilitySpec(*AbilitySpec);
		const FGameplayTag NewInputTag = InputTag;
		const FGameplayTag StatusTag = FindStatusTagByAbilitySpec(*AbilitySpec);

		//  Lock�̳� UnLock Status��� return.
		if (StatusTag.MatchesTagExact(FHASGameplayTags::Get().Status_UnEquipped) || StatusTag.MatchesTagExact(FHASGameplayTags::Get().Status_Equipped))
		{
			// �̹� ������ Input�� �����Ϸ���
			if (FGameplayAbilitySpec* ChangeAbilitySpec = FindAbilitySpecByInputTag(NewInputTag))
			{
				// ���� �������ִ� Ability ���� ����.
				ChangeAbilitySpec->DynamicAbilityTags.RemoveTag(FindInputTagByAbilitySpec(*ChangeAbilitySpec));

				ChangeAbilitySpec->DynamicAbilityTags.RemoveTag(FindStatusTagByAbilitySpec(*ChangeAbilitySpec));
				ChangeAbilitySpec->DynamicAbilityTags.AddTag(FHASGameplayTags::Get().Status_UnEquipped);

				ClientUpdateAbilityInput(*ChangeAbilitySpec, NewInputTag);

				MarkAbilitySpecDirty(*ChangeAbilitySpec);
			}

			// �������ִ� Ability���.
			if (IsEquippedAbility(*AbilitySpec))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(FindInputTagByAbilitySpec(*AbilitySpec));
			}

			AbilitySpec->DynamicAbilityTags.RemoveTag(FindInputTagByAbilitySpec(*AbilitySpec));
			AbilitySpec->DynamicAbilityTags.AddTag(InputTag);
			
			AbilitySpec->DynamicAbilityTags.RemoveTag(FindStatusTagByAbilitySpec(*AbilitySpec));
			AbilitySpec->DynamicAbilityTags.AddTag(FHASGameplayTags::Get().Status_Equipped);
		}
		ClientUpdateAbilityInput(*AbilitySpec, CurInputTag);

		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UHASAbilitySystemComponent::ClientUpdateAbilityInput_Implementation(FGameplayAbilitySpec InAbilitySpec, const FGameplayTag& PrevInputTag)
{
	AbilityEquipDelegate.Broadcast(InAbilitySpec, PrevInputTag);
	AbilityUpdateDelegate.Broadcast(InAbilitySpec);
}

void UHASAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bIsGivenStartAbilities)
	{
		bIsGivenStartAbilities = true;
		StartAbilitiesGivenDelegate.Broadcast();
	}
}

FGameplayAbilitySpec* UHASAbilitySystemComponent::FindAbilitySpecByInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock AbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
		{
			if (Tag.MatchesTagExact(InputTag))
			{
				return &AbilitySpec;
			}
		}
	}

	return nullptr;
}

FGameplayAbilitySpec* UHASAbilitySystemComponent::FindAbilitySpecByAbilityTag(const FGameplayTag& AbilityTag)
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
		for (const FGameplayTag& AbilityTag : AbilitySpec.Ability->AbilityTags)
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
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UHASAbilitySystemComponent::FindInputTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
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

bool UHASAbilitySystemComponent::IsEquippedAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	return AbilitySpec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

void UHASAbilitySystemComponent::ClearInput(FGameplayAbilitySpec* AbilitySpec)
{
	// ���� ����.(Remove input Tag)
	AbilitySpec->DynamicAbilityTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

void UHASAbilitySystemComponent::UnEquipAbility(FGameplayAbilitySpec* AbilitySpec)
{
	// Status - UnEquipped�� ����.
	AbilitySpec->DynamicAbilityTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Status")));
	AbilitySpec->DynamicAbilityTags.AddTag(FHASGameplayTags::Get().Status_UnEquipped);
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

