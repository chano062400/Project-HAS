#include "Player/HASPlayerState.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Net/UnrealNetwork.h"

AHASPlayerState::AHASPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UHASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UHASAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AHASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AHASPlayerState, Level, COND_None, REPNOTIFY_Always);
}

UAttributeSet* AHASPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AHASPlayerState::OnRep_Level(int32 OldLevel)
{

}
