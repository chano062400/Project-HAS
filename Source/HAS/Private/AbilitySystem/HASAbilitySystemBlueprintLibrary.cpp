#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "HASAbilityTypes.h"
#include "Interfaces/HASCombatInterface.h"

void UHASAbilitySystemBlueprintLibrary::SetCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->SetCriticalHit(true);
	}
}

bool UHASAbilitySystemBlueprintLibrary::IsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHASGameplayEffectContext* HASEffectContext = static_cast<const FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HASEffectContext->IsCriticalHit();
	}
	return false;
}

void UHASAbilitySystemBlueprintLibrary::GetPlayersWithinRadius(UObject* WorldContextObject, TArray<AActor*>& OutOverlapingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllDynamicObjects, FCollisionShape::MakeSphere(Radius), CollisionParams);
		for (FOverlapResult& Result : Overlaps)
		{
			if (Result.GetActor()->Implements<UHASCombatInterface>() && !IHASCombatInterface::Execute_IsDead(Result.GetActor()))
			{
				OutOverlapingActors.AddUnique(Result.GetActor());
			}
		}
	}
}

bool UHASAbilitySystemBlueprintLibrary::IsFriend(AActor* Actor1, AActor* Actor2)
{
	if (Actor1->ActorHasTag(FName("Enemy")) && Actor2->ActorHasTag(FName("Enemy"))) return true;
	else if (Actor1->ActorHasTag(FName("Player")) && Actor2->ActorHasTag(FName("Player"))) return true;
	else return false;
}
