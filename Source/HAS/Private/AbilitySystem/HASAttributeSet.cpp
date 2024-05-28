#include "AbilitySystem/HASAttributeSet.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"

UHASAttributeSet::UHASAttributeSet()
{
	
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

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, GetHealthAttribute().GetName());
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Blue, GetManaAttribute().GetName());
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
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

		if (OutProps.TargetController)
		{
			OutProps.TargetCharacter = Cast<ACharacter>(OutProps.TargetController->GetPawn());
		}

		OutProps.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProps.TargetAvatarActor);
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
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, Intelligence, OIdDexterity);
}

void UHASAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OIdCriticalChacne)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, Intelligence, OIdCriticalChacne);
}

void UHASAttributeSet::OnRep_CriticalResistance(const FGameplayAttributeData& OIdCriticalResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHASAttributeSet, Intelligence, OIdCriticalResistance);

}
