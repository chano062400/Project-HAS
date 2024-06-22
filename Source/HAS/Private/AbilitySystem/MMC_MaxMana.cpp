#include "AbilitySystem/MMC_MaxMana.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Interfaces/HASCombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
    IntelligenceDef.AttributeToCapture = UHASAttributeSet::GetIntelligenceAttribute();
    IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

    RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = SourceTags;
    EvaluateParams.TargetTags = TargetTags;

    float Intelligence = 0.f;
    GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParams, Intelligence);
    Intelligence = FMath::Max<float>(0.f, Intelligence);

    UObject* TargetActor = Spec.GetEffectContext().GetSourceObject();

    int32 Level = 1;
    if (TargetActor->Implements<UHASCombatInterface>())
    {
        Level = IHASCombatInterface::Execute_GetLevel(TargetActor);
    }

    return 15.f + Intelligence * 2.5f + Level * 10.f;
}
