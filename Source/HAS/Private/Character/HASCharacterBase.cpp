#include "Character/HASCharacterBase.h"

AHASCharacterBase::AHASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* AHASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHASCharacterBase::InitializeStartAttributes()
{
}

void AHASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

