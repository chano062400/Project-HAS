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

	DOREPLIFETIME(AHASPlayerState, Level);
	DOREPLIFETIME(AHASPlayerState, XP);
	DOREPLIFETIME(AHASPlayerState, AttributePoint);
	DOREPLIFETIME(AHASPlayerState, SpellPoint);
}

UAttributeSet* AHASPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AHASPlayerState::OnRep_Level(int32 OldLevel)
{
	PlayerLevelChangedDelegate.Broadcast(Level);
}

void AHASPlayerState::SetLevel(int32 NewLevel)
{
	Level = NewLevel;
	PlayerLevelChangedDelegate.Broadcast(Level);
}

void AHASPlayerState::OnRep_XP(int32 OldXP)
{
	PlayerXPChangedDelegate.Broadcast(XP);
}

void AHASPlayerState::SetXP(int32 NewXP)
{
	XP = NewXP;
	PlayerXPChangedDelegate.Broadcast(XP);
}

void AHASPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	PlayerAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void AHASPlayerState::SetAttributePoint(int32 NewAttributePoint)
{
	AttributePoint = NewAttributePoint;
	PlayerAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void AHASPlayerState::OnRep_SpellPoint(int32 OldSpellPoint)
{
	PlayerSpellPointChangedDelegate.Broadcast(SpellPoint);
}

void AHASPlayerState::SetSpellPoint(int32 NewSpellPoint)
{
	SpellPoint = NewSpellPoint;
	PlayerSpellPointChangedDelegate.Broadcast(SpellPoint);
}
