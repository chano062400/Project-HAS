#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "HASAbilityTypes.h"
#include "Interfaces/HASCombatInterface.h"
#include "AbilitySystem/Data/ClassInfoDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "HAS/HASGameModeBase.h"
#include "AbilitySystem/Data/LevelXPInfo.h"
#include "Player/HASPlayerState.h"
#include "Interfaces/HASCombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HASGameplayTags.h"

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

bool UHASAbilitySystemBlueprintLibrary::IsApplyDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHASGameplayEffectContext* HASEffectContext = static_cast<const FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->IsApplyDebuff();
	}
	return false;
}

void UHASAbilitySystemBlueprintLibrary::SetIsApplyDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bIsApplyDebuff)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->SetIsApplyDebuff(bIsApplyDebuff);
	}
}

FGameplayTag UHASAbilitySystemBlueprintLibrary::GetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (HASEffectContext->GetDamageType().IsValid())
		{
			return *HASEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

void UHASAbilitySystemBlueprintLibrary::SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		HASEffectContext->SetDamageType(DamageType);
	}
}

float UHASAbilitySystemBlueprintLibrary::GetDebuffChance(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->GetDebuffChance();
	}
	return 0.f;
}

void UHASAbilitySystemBlueprintLibrary::SetDebuffChance(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffChance)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->SetDebuffChance(InDebuffChance);
	}
}

float UHASAbilitySystemBlueprintLibrary::GetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

void UHASAbilitySystemBlueprintLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

float UHASAbilitySystemBlueprintLibrary::GetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

void UHASAbilitySystemBlueprintLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

float UHASAbilitySystemBlueprintLibrary::GetDebuffDamge(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

void UHASAbilitySystemBlueprintLibrary::SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FHASGameplayEffectContext* HASEffectContext = static_cast<FHASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HASEffectContext->SetDebuffDamage(InDebuffDamage);
	}
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

FGameplayEffectContextHandle UHASAbilitySystemBlueprintLibrary::ApplyDamageEffectParams(const FHASDamageEffectParams& Params)
{
	const AActor* SourceAvatarActor = Params.SourceASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = Params.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Params.WorldContextObejct);

	FGameplayEffectSpecHandle EffectSpecHandle = Params.SourceASC->MakeOutgoingSpec(Params.DamageEffectClass, Params.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Params.DamageType, Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, FHASGameplayTags::Get().Debuff_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, FHASGameplayTags::Get().Debuff_Duration, Params.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, FHASGameplayTags::Get().Debuff_Frequency, Params.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, FHASGameplayTags::Get().Debuff_Damage, Params.DebuffDamage);

	Params.TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	return EffectContextHandle;
}
