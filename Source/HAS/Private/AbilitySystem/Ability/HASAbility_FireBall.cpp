#include "AbilitySystem/Ability/HASAbility_FireBall.h"
#include "Interfaces/HASCombatInterface.h"
#include "Actor/HASProjectile.h"
#include "AbilitySystemComponent.h"

//void UHASAbility_FireBall::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bIsHoming)
//{
//	bool bIsServer = GetCurrentActorInfo()->PlayerController->HasAuthority();
//	if (!bIsServer) return;
//
//	if (GetAvatarActorFromActorInfo()->Implements<UHASCombatInterface>())
//	{
//		FVector SocketLocation = IHASCombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
//
//		FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
//		Rotation.Pitch = 0.f; // 직선으로 날아감.
//
//		FTransform SpawnTransform;
//		SpawnTransform.SetRotation(Rotation.Quaternion());
//		SpawnTransform.SetLocation(SocketLocation);
//
//		AHASProjectile* Projectile = GetWorld()->SpawnActorDeferred<AHASProjectile>(
//			ProjectileClass,
//			SpawnTransform,
//			GetOwningActorFromActorInfo(),
//			Cast<APawn>(GetAvatarActorFromActorInfo()),
//			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
//
//		// HASGameplayEffectContext.
//		FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
//		EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
//		Projectile->DamageEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
//
//		Projectile->FinishSpawning(SpawnTransform);
//	}
//}
