#include "Character/HASCharacterBase.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "HASGameplayTags.h"

AHASCharacterBase::AHASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
}

UAbilitySystemComponent* AHASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHASCharacterBase::InitializeStartAttributes()
{
	ApplyAttributes(StartPrimrayAttribute, this);

	ApplyAttributes(StartSecondaryAttribute, this);

	ApplyAttributes(StartVitalAttribute, this);
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

void AHASCharacterBase::HighlightActor()
{
	if (Weapon && GetMesh())
	{
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(250);

		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(250);
	}
}

void AHASCharacterBase::UnHighlightActor()
{
	if (Weapon && GetMesh())
	{
		GetMesh()->SetRenderCustomDepth(false);
		Weapon->SetRenderCustomDepth(false);
	}
}

FVector AHASCharacterBase::GetWeaponSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
	if (SocketTag.MatchesTagExact(FHASGameplayTags::Get().WeaponSocket_Staff))
	{
		return Weapon->GetSocketLocation(FName("Staff"));
	}
	return FVector();
}

void AHASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FAttackMontage AHASCharacterBase::GetAttackMontageInfo(const FGameplayTag& MontageTag)
{
	for (FAttackMontage Info : AttackMontageInfo)
	{
		if (Info.MontageTag.MatchesTagExact(MontageTag))
		{
			return Info;
		}
	}

	return FAttackMontage();
}

