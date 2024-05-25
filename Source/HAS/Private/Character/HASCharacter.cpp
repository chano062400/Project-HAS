#include "Character/HASCharacter.h"
#include "Player/HASPlayerState.h"
#include "Player/HASPlayerController.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "UI/HUD/HASHUD.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"

AHASCharacter::AHASCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("HairGroom"));
	Hair->SetupAttachment(GetMesh(), FName("HairGroom"));
}

void AHASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

}

void AHASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();

}

void AHASCharacter::InitializeStartAttributes()
{
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(StartVitalAttribute, 1.f, EffectContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
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
	AttributeSet = HASPlayerState->GetAttributeSet();

	// BroadcastInitialValue하기 전에 StartAttributeEffect 적용.
	InitializeStartAttributes();

	if (AHASPlayerController* HASPlayerController = Cast<AHASPlayerController>(GetController()))
	{
		if (AHASHUD* HASHUD = Cast<AHASHUD>(HASPlayerController->GetHUD()))
		{
			HASHUD->InitOverlay(HASPlayerController, HASPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

}
