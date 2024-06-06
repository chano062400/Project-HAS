#include "Character/HASEnemy.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Player/HASPlayerState.h"

AHASEnemy::AHASEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UHASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UHASAttributeSet>(TEXT("AttributeSet"));
}

void AHASEnemy::InitAbilityActorInfo()
{

}

void AHASEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (HasAuthority()) InitializeStartAttributes();
}

