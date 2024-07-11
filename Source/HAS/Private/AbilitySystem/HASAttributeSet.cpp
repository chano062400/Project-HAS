#include "AbilitySystem/HASAttributeSet.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "HASGameplayTags.h"
#include "Interfaces/HASCombatInterface.h"
#include "Player/HASPlayerController.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "Interfaces/HASEnemyInterface.h"
#include "Interfaces/HASPlayerInterface.h"
#include "HASAbilityTypes.h"

UHASAttributeSet::UHASAttributeSet()
{
	const FHASGameplayTags& Tag = FHASGameplayTags::Get();

	/* Primary Attributes */
	TagToAttribute.Add( Tag.Attribute_Primary_Intelligence, GetIntelligenceAttribute() );
	TagToAttribute.Add( Tag.Attribute_Primary_Dexterity, GetDexterityAttribute() );
	TagToAttribute.Add( Tag.Attribute_Primary_Vigor, GetVigorAttribute() );

	/* Secondary Attributes */
	TagToAttribute.Add (Tag.Attribute_Secondary_CriticalResistance, GetCriticalResistanceAttribute());
	TagToAttribute.Add( Tag.Attribute_Secondary_CriticalChance, GetCriticalChanceAttribute() );
	TagToAttribute.Add( Tag.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute() );
	TagToAttribute.Add( Tag.Attribute_Secondary_MaxMana, GetMaxManaAttribute() );

	/* Vital Attributes */
	TagToAttribute.Add( Tag.Attribute_Vital_Health, GetHealthAttribute() );
	TagToAttribute.Add( Tag.Attribute_Vital_Mana, GetManaAttribute() );

}

void UHASAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* Vital Attribute */

	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	/* Primary Attribute */

	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);

	/* Secondary Attribute */

	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, CriticalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHASAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

}

void UHASAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProps;
	SetEffectProps(Data, EffectProps);

	/*if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, GetHealthAttribute().GetName());
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}*/
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		HandleIncomingDamage(EffectProps);
	}
	/*if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Blue, GetManaAttribute().GetName());
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}*/
}

void UHASAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// 레벨업하면 MaxHealth,MaxMana도 증가. 
	// MaxHealth 공식 - 50.f + Vigor * 5.f + Level * 25.f;

	if (Attribute == GetMaxHealthAttribute() && bLevelUp)
	{
		SetHealth(GetMaxHealth());
	}
	if (Attribute == GetMaxManaAttribute() && bLevelUp)
	{
		SetMana(GetMaxMana());

		// MaxMana가 MaxHealth보다 늦게 변경되므로, MaxMana가 변경되고서 bLevelUp을 false로 설정.
		bLevelUp = false;
	}
}

void UHASAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}


	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UHASAttributeSet::SetEffectProps(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps)
{
	OutProps.EffectContextHandle = Data.EffectSpec.GetEffectContext();

	OutProps.SourceASC = OutProps.EffectContextHandle.GetInstigatorAbilitySystemComponent();

	if (IsValid(OutProps.SourceASC) && OutProps.SourceASC->AbilityActorInfo.IsValid() && OutProps.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProps.SourceAvatarActor = OutProps.SourceASC->GetAvatarActor();

		APlayerController* SourcePC = OutProps.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (SourcePC == nullptr && OutProps.SourceAvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(OutProps.SourceAvatarActor))
			{
				OutProps.SourceController = Pawn->GetController();
			}
		}

		if (OutProps.SourceController)
		{
			OutProps.SourceCharacter = Cast<ACharacter>(OutProps.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProps.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();

		OutProps.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

		if (OutProps.TargetAvatarActor)
		{
			OutProps.TargetCharacter = Cast<ACharacter>(OutProps.TargetAvatarActor);
		}

		OutProps.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProps.TargetAvatarActor);
	}
}

void UHASAttributeSet::HandleIncomingDamage(FEffectProperties& Props)
{
	if (Props.SourceAvatarActor == Props.TargetAvatarActor) return;

	const float LocalIncomingDamage = GetInComingDamage();
	SetInComingDamage(0.f);

	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		const bool bFatal = NewHealth <= 0.f;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		ShowDamageText(Props, LocalIncomingDamage);

		if (bFatal)
		{
			HandleXP(Props);
		}
		// HitReact
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FHASGameplayTags::Get().Ability_HitReact);

			OnAttackedDelegate.Broadcast(Props.SourceAvatarActor);

			// TagContainer에 추가된 Tag와 맞는 Ability를 Activate.
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);

			const bool bIsApplyDebuff = UHASAbilitySystemBlueprintLibrary::IsApplyDebuff(Props.EffectContextHandle);
			if (bIsApplyDebuff)
			{
				HandleDebuff(Props);
			}

			/*float KnockbackForce = UHASAbilitySystemBlueprintLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (KnockbackForce > 0.f)
			{
				
				Props.SourceCharacter->SetActorLocation()
			}*/
		}
	}
}

void UHASAttributeSet::HandleXP(FEffectProperties& Props)
{
	if (IHASCombatInterface* CombatInterface = Cast<IHASCombatInterface>(Props.TargetAvatarActor))
	{
		CombatInterface->Die();
		if (Props.TargetAvatarActor->Implements<UHASEnemyInterface>() && Props.SourceAvatarActor->Implements<UHASPlayerInterface>())
		{
			IHASEnemyInterface* EnemyInterface = Cast<IHASEnemyInterface>(Props.TargetAvatarActor);

			FClassDefaultInfo Info = UHASAbilitySystemBlueprintLibrary::GetClassDefaultInfo(Props.TargetAvatarActor, EnemyInterface->GetCharacterClass());
			int32 EnemyLevel = CombatInterface->Execute_GetLevel(Props.TargetAvatarActor);
			int32 EnemyXPReward = Info.XPReward.GetValueAtLevel(EnemyLevel);

			IHASPlayerInterface* PlayerInterface = Cast<IHASPlayerInterface>(Props.SourceAvatarActor);
			int32 PlayerLevel = CombatInterface->Execute_GetLevel(Props.SourceAvatarActor);

			int32 PlayerNewXP = PlayerInterface->GetXP() + EnemyXPReward;
			int32 PlayerNewLevel = UHASAbilitySystemBlueprintLibrary::GetLevelByXP(Props.SourceAvatarActor, PlayerNewXP);
			int32 PlayerAttributePoint = PlayerInterface->GetAttributePoint();
			int32 PlayerSpellPoint = PlayerInterface->GetSpellPoint();

			const int32 NumOfLevelUp = PlayerNewLevel - PlayerLevel;

			// 레벨업 했다면
			if (NumOfLevelUp > 0)
			{
				// MMC_MaxHealth,Mana 공식에 Level 값이 있으므로, 변경 전에 Level을 올려줌.
				PlayerInterface->SetLevel(PlayerNewLevel);
				PlayerInterface->SetAttributePoint(PlayerAttributePoint + 5 * NumOfLevelUp);
				PlayerInterface->SetSpellPoint(PlayerSpellPoint + 3 * NumOfLevelUp);

				// 핵심 스텟 + 1, Vigor, Intelligence 값이 변경됨에 따라 MMC_MaxHealth,Mana를 호출하여 MaxHealth,Mana Attribute값 변경 -> PostAttributeChange() 호출
				Props.SourceASC->ApplyModToAttribute(GetVigorAttribute(), EGameplayModOp::Additive, NumOfLevelUp);
				Props.SourceASC->ApplyModToAttribute(GetIntelligenceAttribute(), EGameplayModOp::Additive, NumOfLevelUp);
				Props.SourceASC->ApplyModToAttribute(GetDexterityAttribute(), EGameplayModOp::Additive, NumOfLevelUp);

			}

			PlayerInterface->SetXP(PlayerNewXP);
		}
	}
}

//Debuff Effect 적용.
void UHASAttributeSet::HandleDebuff(FEffectProperties& Props)
{
	const FGameplayTag DamageType = UHASAbilitySystemBlueprintLibrary::GetDamageType(Props.EffectContextHandle);

	const FString DebuffName = FString::Printf(TEXT("%s Debuff"), *DamageType.ToString());
	const float DebuffChance = UHASAbilitySystemBlueprintLibrary::GetDebuffChance(Props.EffectContextHandle);
	const float DebuffDuration = UHASAbilitySystemBlueprintLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UHASAbilitySystemBlueprintLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	const float DebuffDamage = UHASAbilitySystemBlueprintLibrary::GetDebuffDamge(Props.EffectContextHandle);

	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->DurationMagnitude = FGameplayEffectModifierMagnitude(DebuffDuration);
	Effect->Period = DebuffFrequency;
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const FGameplayTag DebuffTag = FHASGameplayTags::Get().DamageToDebuff[DamageType];
	Effect->InheritableOwnedTagsContainer.AddTag(DebuffTag);

	FGameplayEffectContextHandle EffectContextHandle = Props.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Props.SourceAvatarActor);

	const int32 Idx = Effect->Modifiers.Num();
	// 적용할 Modifier
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Idx];
	ModifierInfo.Attribute = GetInComingDamageAttribute();
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	if (FGameplayEffectSpec* Spec = new FGameplayEffectSpec(Effect, EffectContextHandle, 1.f))
	{
		FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		HASEffectContext->SetDamageType(DebuffDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

void UHASAttributeSet::ShowDamageText(FEffectProperties& Props, float Damage)
{
	bool bIsCritical = UHASAbilitySystemBlueprintLibrary::IsCriticalHit(Props.EffectContextHandle);

	// Player가 공격할 때
	if (AHASPlayerController* PC = Cast<AHASPlayerController>(Props.SourceASC->AbilityActorInfo.Get()->PlayerController))
	{
		PC->ClientShowFloatingDamageText(Damage, Props.TargetAvatarActor, bIsCritical);
	}

	// Enemy가 공격할 때
	if (AHASPlayerController* PC = Cast<AHASPlayerController>(Props.TargetController))
	{
		PC->ClientShowFloatingDamageText(Damage, Props.TargetAvatarActor, bIsCritical);
	}
}

void UHASAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, Health, OldHealth);
}

void UHASAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, MaxHealth, OldMaxHealth);
}

void UHASAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, Mana, OldMana);
}

void UHASAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, MaxMana, OldMaxMana);
}

void UHASAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, Vigor, OldVigor);
}

void UHASAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OIdIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, Intelligence, OIdIntelligence);
}

void UHASAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OIdDexterity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, Dexterity, OIdDexterity);
}

void UHASAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OIdCriticalChacne)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, CriticalChance, OIdCriticalChacne);
}

void UHASAttributeSet::OnRep_CriticalResistance(const FGameplayAttributeData& OIdCriticalResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, CriticalResistance, OIdCriticalResistance);

}
