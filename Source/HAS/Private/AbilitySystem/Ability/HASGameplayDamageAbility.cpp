#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "Actor/HASProjectile.h"
#include "Interfaces/HASCombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HASAbilityTypes.h"

void UHASGameplayDamageAbility::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bIsHoming)
{
	if (GetCurrentActorInfo()->PlayerController.IsValid())
	{
		bool bIsServer = GetCurrentActorInfo()->PlayerController->HasAuthority();
		if (!bIsServer) return;
	}

	if (GetAvatarActorFromActorInfo()->Implements<UHASCombatInterface>())
	{
		FVector SocketLocation = IHASCombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);

		FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetRotation(Rotation.Quaternion());
		SpawnTransform.SetLocation(SocketLocation);

		Projectile = GetWorld()->SpawnActorDeferred<AHASProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		Projectile->DamageEffectParams = MakeDamageEffectParams(nullptr);
		Projectile->DamageEffectParams.SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UHASGameplayDamageAbility::ApplyDamage(AActor* CombatTarget, int32 Level)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	
		const float	DamageMagnitude = Damage.GetValueAtLevel(Level);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, DamageType, DamageMagnitude);
		
		ASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CombatTarget));
	}
}

FHASDamageEffectParams UHASGameplayDamageAbility::MakeDamageEffectParams(AActor* TargetActor)
{
	FHASDamageEffectParams Params;

	Params.WorldContextObejct = GetAvatarActorFromActorInfo();
	Params.AbilityLevel = GetAbilityLevel();
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	if(IsValid(TargetActor)) Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.DamageEffectClass = DamageEffectClass;
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DebuffDamage = DebuffDamage;
	Params.KnockbackChance = KnockbackChance;
	Params.KnockbackForce = KnockbackForce;

	return Params;
}
