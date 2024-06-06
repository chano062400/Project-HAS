#include "AbilitySystem/HASDamageExecution.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "HASGameplayTags.h"
#include "Interfaces/HASCombatInterface.h"

//Raw Struct로 Blueprint나 Reflection System 어느 곳에도 노출시키지 않을 것이라서 F 안붙임.
struct HASDamageStatics
{
	//존재하지 않는 변수를 넣어도 알아서 FProperty형 포인터로 만들어서 CaptureDefinition으로 만들어 줌.
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence)

	HASDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHASAttributeSet, CriticalChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHASAttributeSet, CriticalResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHASAttributeSet, Intelligence, Source, false);
	}
};

static const HASDamageStatics& DamageStatics()
{
	static HASDamageStatics Statics;
	return Statics;
}

UHASDamageExecution::UHASDamageExecution()
{

}

void UHASDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatarActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatarActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters Params;
	Params.SourceTags = SourceTags;
	Params.TargetTags = TargetTags;

	int32 SourceActorLevel = 1;
	if (IsValid(SourceAvatarActor) && SourceAvatarActor->Implements<UHASCombatInterface>())
	{
		SourceActorLevel = IHASCombatInterface::Execute_GetLevel(SourceAvatarActor);
	}

	float Intelligence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().IntelligenceDef, Params, Intelligence);
	Intelligence = FMath::Max<float>(0.f, Intelligence);

	float BaseDamage = SourceActorLevel * 10 + Intelligence * 2.5;

	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalChanceDef, Params, SourceCriticalChance);
	SourceCriticalChance = FMath::Max<float>(0.f, SourceCriticalChance);

	float TargetCriticalResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalResistanceDef, Params, TargetCriticalResistance);
	TargetCriticalResistance = FMath::Max<float>(0.f, TargetCriticalResistance);

	float EffectiveCriticalChance = SourceCriticalChance - TargetCriticalResistance;
	bool bIsCritical = EffectiveCriticalChance > FMath::RandRange(1, 100);
	if (bIsCritical) BaseDamage *= 1.25f;

	const FGameplayModifierEvaluatedData EvaluatedData(UHASAttributeSet::GetInComingDamageAttribute(), EGameplayModOp::Additive, BaseDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
