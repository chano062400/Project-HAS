#include "Character/HASCharacterBase.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "HASGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"

AHASCharacterBase::AHASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	BurnDebuffComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Burn Debuff Component"));
	BurnDebuffComponent->SetupAttachment(GetMesh());
	BurnDebuffComponent->bAutoActivate = false;

	ElectricShockDebuffComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Electric Shock Debuff Component"));
	ElectricShockDebuffComponent->SetupAttachment(GetMesh());
	ElectricShockDebuffComponent->bAutoActivate = false;

	DebuffTagToNiagara.Add({ FHASGameplayTags::Get().Debuff_Burn, BurnDebuffComponent});
	DebuffTagToNiagara.Add({ FHASGameplayTags::Get().Debuff_ElectricShock, ElectricShockDebuffComponent});
	DebuffTagToNiagara.Add({ FHASGameplayTags::Get().Debuff_Freeze, nullptr });
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
	if (SocketTag.MatchesTagExact(FHASGameplayTags::Get().WeaponSocket_Bow))
	{
		return Weapon->GetSocketLocation(FName("Bow"));
	}
	return FVector();
}

void AHASCharacterBase::HitReactTagEvent(const FGameplayTag Tag, int32 NewCount)
{
	//NewCount ? GetCharacterMovement()->MaxWalkSpeed = 0.f : GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AHASCharacterBase::MulticastFreezeDebuffHandle_Implementation(bool bIsFrozen)
{
	if (bIsFrozen)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		PlayFreezeEffect();
	}
	else
	{
		ReturnToDefaultMaterial();
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	}
}

void AHASCharacterBase::DebuffTagEvent(const FGameplayTag Tag, int32 NewCount)
{
	for (TTuple<FGameplayTag, UNiagaraComponent*> pair : DebuffTagToNiagara)
	{
		if (Tag.MatchesTagExact(FHASGameplayTags::Get().Debuff_Freeze))
		{
			if (NewCount > 0)
			{
				MulticastFreezeDebuffHandle(true);
			}
			else
			{
				MulticastFreezeDebuffHandle(false);
			}
		}
		else
		{
			if (Tag.MatchesTagExact(pair.Key))
			{
				NewCount > 0 ? pair.Value->Activate() : pair.Value->Deactivate();
			}
		}
	}
}

void AHASCharacterBase::Die()
{
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->RemoveAllDebuffEffect();
	}
	MulticastHandleDie();
}

void AHASCharacterBase::MulticastHandleDie_Implementation()
{
	Weapon->DestroyComponent();

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayDieEffect(DieEffectMaterialIndex);
	bDead = true;
}

void AHASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FMontageInfo AHASCharacterBase::GetMontageInfoByTag_Implementation(const FGameplayTag& MontageTag)
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

bool AHASCharacterBase::IsDead_Implementation() const
{
	return bDead;
}
