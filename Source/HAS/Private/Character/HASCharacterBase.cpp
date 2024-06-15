#include "Character/HASCharacterBase.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "HASGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AHASCharacterBase::AHASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

UAbilitySystemComponent* AHASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHASCharacterBase::InitializeDefaultAttributesByClass(ECharacterClass InCharacterClass, int32 Level)
{
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->ApplyDefaultAttributesByClass(InCharacterClass, Level);
	}
}

void AHASCharacterBase::AddDefaultAbilitiesByClass(ECharacterClass InCharacterClass, int32 Level)
{
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AddDefaultAbilitiesByClass(InCharacterClass, Level);
	}
}

void AHASCharacterBase::InitializeDefaultAttributes()
{
	ApplyAttribute(DefaultPrimrayAttribute, this);
	ApplyAttribute(DefaultSecondaryAttribute, this);
	ApplyAttribute(DefaultVitalAttribute, this);

}

void AHASCharacterBase::ApplyAttribute(TSubclassOf<UGameplayEffect> Attribute, AActor* SourceObject)
{
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceObject);

	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Attribute, 1.f, EffectContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void AHASCharacterBase::InitAbilityActorInfo()
{
}

void AHASCharacterBase::AddStartAbilities()
{
	if (!HasAuthority()) return;
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AddStartAbilitiesByInputTag(StartAbilities);
	}
}

void AHASCharacterBase::AddCommonAbilities()
{
	if (!HasAuthority()) return;
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AddCommonAbilities();
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

void AHASCharacterBase::HitReactTagEvent(const FGameplayTag Tag, int32 NewCount)
{
	NewCount ? GetCharacterMovement()->MaxWalkSpeed = 0.f : GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AHASCharacterBase::Die()
{
	MulticastHandleDie();
}

void AHASCharacterBase::MulticastHandleDie_Implementation()
{
	FDetachmentTransformRules Rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	Weapon->DetachFromComponent(Rules);
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayDieEffect(DieEffectMaterialIndex);
}

void AHASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FMontageInfo AHASCharacterBase::GetMontageInfo_Implementation(const FGameplayTag& MontageTag)
{
	for (FMontageInfo Info : MontageInformations)
	{
		if (Info.MontageTag.MatchesTagExact(MontageTag))
		{
			return Info;
		}
	}

	return FMontageInfo();
}

