#include "AbilitySystem/Ability/HASAbility_IceBeam.h"
#include "HASAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HASCombatInterface.h"
#include "HASGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"

FString UHASAbility_IceBeam::GetAbilityDescription(int32 InAbilityLevel)
{
	float ManaCost = GetManaCost(InAbilityLevel);
	float Cooldown = GetCooldown(InAbilityLevel);

	return FString::Printf(TEXT(
		"<Title> Ice Beam </> \n\n"
		
		"<Level> Rank : %d / 5 </> \n\n"
		
		"While pressed, releases an IceBeam in the direction of the player's mouse, dealing <Damage>%.2f</> damage and applying a debuff with a <Debuff>%.2f</> chance\n\n"
	
		" Cost : <Cost>%.2f</> \n"

		" Cooldown : <Cooldown>%.2f</>"
	),
		InAbilityLevel,
		Damage.GetValueAtLevel(InAbilityLevel),
		DebuffChance,
		-ManaCost,
		Cooldown
	);
}

void UHASAbility_IceBeam::BeamTrace(float BeamRadius, float BeamLength)
{
	if (GetCurrentActorInfo()->PlayerController.IsValid())
	{
		bool bIsServer = GetCurrentActorInfo()->PlayerController->HasAuthority();
		if (!bIsServer) return;
	}

	TArray<FOverlapResult> OutOverlaps;

	FCollisionQueryParams Params(NAME_None, false, GetAvatarActorFromActorInfo());
	
	if (GetAvatarActorFromActorInfo()->Implements<UHASCombatInterface>())
	{	
		const FVector StaffLoc = IHASCombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo(), FHASGameplayTags::Get().WeaponSocket_Staff);
		StaffLoc.Rotation() = UKismetMathLibrary::MakeRotFromZ(GetAvatarActorFromActorInfo()->GetActorForwardVector());

		const bool bResult = GetWorld()->OverlapMultiByChannel(
			OutOverlaps,
			StaffLoc + GetAvatarActorFromActorInfo()->GetActorForwardVector() * BeamLength * 0.5f,
			UKismetMathLibrary::MakeRotFromZ(GetAvatarActorFromActorInfo()->GetActorForwardVector()).Quaternion(),
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeCapsule(BeamRadius, BeamLength * 0.5f + BeamRadius),
			Params
		);

//#if ENABLE_DRAW_DEBUG
//
//		FVector TraceVec = GetAvatarActorFromActorInfo()->GetActorForwardVector() * BeamLength;
//		FVector Center = StaffLoc + TraceVec * 0.5f;
//		float HalfHeight = BeamLength * 0.5f + BeamRadius;
//		FQuat CapsuleRot = UKismetMathLibrary::MakeRotFromZ(GetAvatarActorFromActorInfo()->GetActorForwardVector()).Quaternion();
//		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//		float DebugLifeTime = 3.0f;
//
//		DrawDebugCapsule
//		(
//			GetWorld(),
//			Center,
//			HalfHeight,
//			BeamRadius,
//			CapsuleRot,
//			DrawColor,
//			false,
//			DebugLifeTime
//		);
//
//#endif

		FHASDamageEffectParams DamageEffectParams = MakeDamageEffectParams(nullptr);

		if (OutOverlaps.Num() > 0)
		{
			for (FOverlapResult Result : OutOverlaps)
			{
				if (UHASAbilitySystemBlueprintLibrary::IsFriend(GetAvatarActorFromActorInfo(), Result.GetActor())) continue;

				DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Result.GetActor());

				UHASAbilitySystemBlueprintLibrary::ApplyDamageEffectParams(DamageEffectParams);

			}
		}
	}
}