#include "AbilitySystem/Ability/HASAbility_FireMeteor.h"
#include "Actor/HASProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/HASProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UHASAbility_FireMeteor::SpawnMeteor(const FVector& TargetLocation)
{
	// 생성위치
	FVector SpawnLoc = FVector(TargetLocation.X, TargetLocation.Y, TargetLocation.Z + 1000.f);
	/*DrawDebugSphere(GetWorld(), SpawnLoc, 50.f, 12, FColor::Blue, true);*/

	// 떨어질 곳 랜덤위치.
	float RandValueX = FMath::RandRange(-Radius, Radius);
	float RandValueY = FMath::RandRange(-Radius, Radius);
	FVector EndLoc = FVector(TargetLocation.X + RandValueX, TargetLocation.Y + RandValueY, 0.f);
	FRotator Rot = (EndLoc - SpawnLoc).Rotation();
	FTransform SpawnTransform;
	SpawnTransform.SetRotation(Rot.Quaternion());
	SpawnTransform.SetLocation(SpawnLoc);

	/*DrawDebugLine(GetWorld(), SpawnLoc, EndLoc, FColor::Red, true);
	DrawDebugSphere(GetWorld(), EndLoc, 50.f, 12, FColor::Blue, true);*/

	Projectile = GetWorld()->SpawnActorDeferred<AHASProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	Projectile->DamageEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	const float DamageMagnitude = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Projectile->DamageEffectSpecHandle, DamageType, DamageMagnitude);

	Projectile->FinishSpawning(SpawnTransform);
}
