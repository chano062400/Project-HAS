#include "Character/HASCharacter.h"
#include "Player/HASPlayerState.h"
#include "Player/HASPlayerController.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "UI/HUD/HASHUD.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "HASGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Actor/HASItem.h"
#include "Inventory/HASInventoryComponent.h"
#include "UI/WIdget/HASUserWidget.h"

AHASCharacter::AHASCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("HairGroom"));
	Hair->SetupAttachment(GetMesh(), FName("HairGroom"));

	LevelUpEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUp Effect"));
	LevelUpEffectComponent->SetupAttachment(GetRootComponent());
	LevelUpEffectComponent->bAutoActivate = false;

	Inventory = CreateDefaultSubobject<UHASInventoryComponent>(TEXT("Inventory"));
	Inventory->SetIsReplicated(true);
}

void AHASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	AddStartAbilities();

	//AbilitySystemComponent->RegisterGameplayTagEvent(FHASGameplayTags::Get().Ability_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHASCharacterBase::HitReactTagEvent);

}

void AHASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();

	//AbilitySystemComponent->RegisterGameplayTagEvent(FHASGameplayTags::Get().Ability_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHASCharacterBase::HitReactTagEvent);

}

void AHASCharacter::InitAbilityActorInfo()
{
	AHASPlayerState* HASPlayerState = GetPlayerState<AHASPlayerState>();

	checkf(HASPlayerState, TEXT("Can't find PlayerState"));

	HASPlayerState->AbilitySystemComponent->InitAbilityActorInfo(HASPlayerState, this);

	// 업 캐스팅
	// HASCHaracter - UAbilitySystemComponent, HASPlayerState - UHASAbilitySystemComponent.
	// HASCHaracter - UAttributeSet,           HASPlayerState - UHASAttributeSet.
	AbilitySystemComponent = HASPlayerState->GetAbilitySystemComponent();
	AttributeSetComp = HASPlayerState->GetAttributeSet();

	// BroadcastInitialValue하기 전에 StartAttributeEffect 적용.
	InitializeDefaultAttributes();

	ApplyRegenerationEffect(RegenerationEffectClass);

	if (AHASPlayerController* HASPlayerController = Cast<AHASPlayerController>(GetController()))
	{
		if (AHASHUD* HASHUD = Cast<AHASHUD>(HASPlayerController->GetHUD()))
		{
			HASHUD->InitOverlay(HASPlayerController, HASPlayerState, AbilitySystemComponent, AttributeSetComp);
		}
	}

}

void AHASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHASCharacter, bCastIceBeamLoop);

}

void AHASCharacter::ApplyRegenerationEffect(TSubclassOf<UGameplayEffect> EffectClass)
{
	if (!HasAuthority()) return;

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, EffectContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void AHASCharacter::MulticastPlayLevelUpEffect_Implementation()
{
	if (IsValid(LevelUpEffectComponent))
	{
		const FVector CameraLoc = Camera->GetComponentLocation();
		const FVector NiagaraLoc = LevelUpEffectComponent->GetComponentLocation();
		const FRotator Rotation = (CameraLoc - NiagaraLoc).Rotation();

		LevelUpEffectComponent->SetWorldRotation(Rotation);
		LevelUpEffectComponent->Activate(true);
	}
}

int32 AHASCharacter::GetLevel_Implementation()
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);
	
	return PS->GetLevel();
}

void AHASCharacter::SetXP(int32 NewXP)
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	PS->SetXP(NewXP);
}

void AHASCharacter::SetLevel(int32 NewLevel)
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	Cast<UHASAttributeSet>(PS->GetAttributeSet())->bLevelUp = true;

	PS->SetLevel(NewLevel);

	MulticastPlayLevelUpEffect();

	if (UHASAbilitySystemComponent* HASASC = Cast<UHASAbilitySystemComponent>(AbilitySystemComponent))
	{
		HASASC->ServerUpdateAbilityStatus(NewLevel);
	}
}

int32 AHASCharacter::GetXP()
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	return PS->GetXP();
}

int32 AHASCharacter::GetAttributePoint()
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	return PS->GetAttributePoint();
}

void AHASCharacter::SetAttributePoint(int32 NewAttributePoint)
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	return PS->SetAttributePoint(NewAttributePoint);
}

int32 AHASCharacter::GetSpellPoint()
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	return PS->GetSpellPoint();
}

void AHASCharacter::SetSpellPoint(int32 NewSpellPoint)
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	return PS->SetSpellPoint(NewSpellPoint);
}

UAttributeSet* AHASCharacter::GetAttributeSet()
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	return PS->GetAttributeSet();
}

AHASMagicCircle* AHASCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	AHASPlayerController* PC = Cast<AHASPlayerController>(GetController());
	check(PC);

	return PC->ShowMagicCircle(DecalMaterial);
}

void AHASCharacter::HideMagicCircle_Implementation()
{
	AHASPlayerController* PC = Cast<AHASPlayerController>(GetController());
	check(PC);

	PC->HideMagicCircle();
}

void AHASCharacter::SetCastIceBeamLoop_Implementation(bool bInCastIcemBeamLoop)
{
	bCastIceBeamLoop = bInCastIcemBeamLoop;
}
