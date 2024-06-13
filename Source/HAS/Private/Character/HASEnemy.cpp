#include "Character/HASEnemy.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystem/HASAttributeSet.h"
#include "Player/HASPlayerState.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/HASUserWidget.h"
#include "HASGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/HASAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AHASEnemy::AHASEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UHASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSetComp = CreateDefaultSubobject<UHASAttributeSet>(TEXT("AttributeSet"));

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetVisibility(false);

}

void AHASEnemy::InitAbilityActorInfo()
{

}

void AHASEnemy::Die()
{
	HealthBarWidget->SetVisibility(false);
	if (HasAuthority()) HASAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	Super::Die();
}

void AHASEnemy::HitReactTagEvent(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		HealthBarWidget->SetVisibility(true);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		HealthBarWidget->SetVisibility(false);
	}
}

void AHASEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	HASAIController = Cast<AHASAIController>(NewController);
	check(HASAIController);

	// BehaviorTree 로드 및 실행.
	HASAIController->RunBehaviorTree(BehaviorTree);
	HASAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	HASAIController->GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), nullptr);
	HASAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DistanceToTarget"), 0.f);
	HASAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), false);
}

void AHASEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	AddDefaultAbilitiesByClass(CharacterClass, Level);

	AddCommonAbilities();

	if (HasAuthority()) InitializeDefaultAttributesByClass(CharacterClass, Level);

	if (UHASUserWidget* HASWidget = Cast<UHASUserWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		HASWidget->SetWidgetController(this);
	}

	if (UHASAttributeSet* AS = Cast<UHASAttributeSet>(AttributeSetComp))
	{
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

		// 초기값
		MaxHealthChangedDelegate.Broadcast(AS->GetMaxHealth());
		HealthChangedDelegate.Broadcast(AS->GetHealth());
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(FHASGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHASEnemy::HitReactTagEvent);

}

