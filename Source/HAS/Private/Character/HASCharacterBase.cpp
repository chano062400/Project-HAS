#include "Character/HASCharacterBase.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"

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

void AHASCharacterBase::InitAbilityActorInfo()
{
}

void AHASCharacterBase::ApplyAttributes(TSubclassOf<UGameplayEffect> EffectClass, AActor* SourceObject)
{
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceObject);

	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, EffectContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void AHASCharacterBase::AddStartAbilities()
{
	if (!HasAuthority()) return;
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AddStartAbilities(StartAbilities);
	}
}

int32 AHASCharacterBase::GetLevel_Implementation()
{
	return 0;
}

void AHASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

