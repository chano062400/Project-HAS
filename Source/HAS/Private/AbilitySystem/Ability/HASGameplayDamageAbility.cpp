#include "AbilitySystem/Ability/HASGameplayDamageAbility.h"
#include "Actor/HASProjectile.h"
#include "Interfaces/HASCombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

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
		Rotation.Pitch = 0.f; // 직선으로 날아감.

		FTransform SpawnTransform;
		SpawnTransform.SetRotation(Rotation.Quaternion());
		SpawnTransform.SetLocation(SocketLocation);

		Projectile = GetWorld()->SpawnActorDeferred<AHASProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		// HASGameplayEffectContext.
		FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
		Projectile->DamageEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
		
		const float DamageMagnitude = Damage.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Projectile->DamageEffectSpecHandle, DamageType, DamageMagnitude);

		Projectile->FinishSpawning(SpawnTransform);
	}
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
