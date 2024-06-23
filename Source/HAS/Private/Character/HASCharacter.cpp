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
	Cast<UHASAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	// 업 캐스팅
	// HASCHaracter - UAbilitySystemComponent, HASPlayerState - UHASAbilitySystemComponent.
	// HASCHaracter - UAttributeSet,           HASPlayerState - UHASAttributeSet.
	AbilitySystemComponent = HASPlayerState->GetAbilitySystemComponent();
	AttributeSetComp = HASPlayerState->GetAttributeSet();

	// BroadcastInitialValue하기 전에 StartAttributeEffect 적용.
	InitializeDefaultAttributes();

	if (AHASPlayerController* HASPlayerController = Cast<AHASPlayerController>(GetController()))
	{
		if (AHASHUD* HASHUD = Cast<AHASHUD>(HASPlayerController->GetHUD()))
		{
			HASHUD->InitOverlay(HASPlayerController, HASPlayerState, AbilitySystemComponent, AttributeSetComp);
		}
	}

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
}

int32 AHASCharacter::GetXP()
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	return PS->GetXP();
}

UAttributeSet* AHASCharacter::GetAttributeSet()
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	check(PS);

	return PS->GetAttributeSet();
}
