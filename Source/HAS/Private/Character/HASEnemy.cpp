#include "Character/HASEnemy.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Player/HASPlayerState.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/HASUserWidget.h"
#include "HASGameplayTags.h"

AHASEnemy::AHASEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UHASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSetComp = CreateDefaultSubobject<UHASAttributeSet>(TEXT("AttributeSet"));

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetIsReplicated(true);

}

void AHASEnemy::InitAbilityActorInfo()
{

}

void AHASEnemy::Die()
{
	HealthBarWidget->SetVisibility(false);
	Super::Die();
}

void AHASEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	AddStartAbilities();

	AddHitReactAbility(HitReactAbility);

	if (HasAuthority()) InitializeStartAttributes();

	AbilitySystemComponent->RegisterGameplayTagEvent(FHASGameplayTags::Get().Ability_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHASCharacterBase::HitReactTagEvent);

	if (UHASUserWidget* HASWidget = Cast<UHASUserWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		HASWidget->SetWidgetController(this);
	}

	if (UHASAttributeSet* AS = Cast<UHASAttributeSet>(AttributeSetComp))
	{
		// �ʱⰪ
		MaxHealthChangedDelegate.Broadcast(AS->GetMaxHealth());
		HealthChangedDelegate.Broadcast(AS->GetHealth());

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				MaxHealthChangedDelegate.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				HealthChangedDelegate.Broadcast(Data.NewValue);
			}
		);
	}

}

