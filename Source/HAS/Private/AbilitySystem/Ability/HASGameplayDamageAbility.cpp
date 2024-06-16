#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "Actor/HASProjectile.h"
#include "Interfaces/HASCombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UHASGameplayDamageAbility::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bIsHoming)
{

}

void UHASGameplayDamageAbility::ApplyDamage(AActor* CombatTarget, int32 Level)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
	{
		FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	
		const float	DamageMagnitude = Damage.GetValueAtLevel(Level);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, DamageType, DamageMagnitude);
		
		ASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CombatTarget));
	}
}
