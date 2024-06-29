#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "HASAbilityTypes.h"
#include "Interfaces/HASCombatInterface.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "HAS/HASGameModeBase.h"
#include "AbilitySystem/Data/LevelXPInfo.h"
#include "Player/HASPlayerState.h"
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

void UHASAbilitySystemBlueprintLibrary::GetActorsWithinRadius(UObject* WorldContextObject, TArray<AActor*>& OutOverlapingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
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

bool UHASAbilitySystemBlueprintLibrary::IsDead(AActor* Actor)
{
	if (Actor->Implements<UHASCombatInterface>())
	{
		bool bIsDead = IHASCombatInterface::Execute_IsDead(Actor);
		return bIsDead;
	}
	return false;
}

FClassDefaultInfo UHASAbilitySystemBlueprintLibrary::GetClassDefaultInfo(UObject* WorldContextObejct, ECharacterClass CharacterClass)
{
	if (AHASGameModeBase* GameMode = Cast<AHASGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObejct)))
	{
		return GameMode->ClassInformation->GetCharacterClassInfo(CharacterClass);
	}

	return FClassDefaultInfo();
}

int32 UHASAbilitySystemBlueprintLibrary::GetLevelByXP(UObject* WorldContextObejct, int32 XP)
{
	if (AHASPlayerState* PS = Cast<AHASPlayerState>(UGameplayStatics::GetPlayerState(WorldContextObejct, 0)))
	{
		return PS->LevelXPInformation->FindLevelByXP(XP);
	}

	return 0;
}

void UHASAbilitySystemBlueprintLibrary::SpawnFireSphere(UObject* WorldContextObject, TArray<FVector>& OutVectors, float Radius, float Interval, const FVector& SphereOrigin)
{
	for (int x = -Radius; x <= Radius; x += Interval)
	{
		for (int y = -Radius; y <= Radius; y += Interval)
		{
			FVector XY = SphereOrigin + FVector(x, y, 0.f);
			float distance = SphereOrigin.Distance(SphereOrigin, XY);

			if (FMath::Sqrt(Radius * Radius) == distance)
			{
				OutVectors.Add(XY);
				//DrawDebugSphere(WorldContextObject->GetWorld(), XY, 50.f, 12, FColor::White, false, 5.f);
			}
		}
	}
}
