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
#include "HASEffectActor.h"

AHASEnemy::AHASEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UHASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSetComp = CreateDefaultSubobject<UHASAttributeSet>(TEXT("AttributeSet"));

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetVisibility(false);

	BaseWalkSpeed = 500.f;
}

void AHASEnemy::InitAbilityActorInfo()
{

}

void AHASEnemy::Die()
{
	HealthBarWidget->SetVisibility(false);
	/*if (HasAuthority()) HASAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);*/
	if (HasAuthority()) HASAIController->BehaviorTree->StopTree();

	bool bDrop = DropChance >= FMath::RandRange(0,100);
	if (bDrop && HasAuthority())
	{
		int32 RandIdx = FMath::RandRange(0, SpawnActorClasses.Num() - 1);
		FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 50.f, GetActorLocation().Z + 50.f);
		AHASEffectActor* SpawnedActor = GetWorld()->SpawnActor<AHASEffectActor>(SpawnActorClasses[RandIdx], SpawnLocation, GetActorRotation());
	}
	Super::Die();
}

void AHASEnemy::HitReactTagEvent(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		HealthBarWidget->SetVisibility(true);
	}
	else
	{
		HealthBarWidget->SetVisibility(false);
	}
}

AActor* AHASEnemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

void AHASEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

FEnemyInfo AHASEnemy::GetTargetInfo_Implementation()
{
	FEnemyInfo EnemyInfo;
	switch (CharacterClass)
	{
		case ECharacterClass::ECC_Archer:
			EnemyInfo.Name = FName("Skeleton Archer");
			break;
		case ECharacterClass::ECC_Warrior:
			EnemyInfo.Name = FName("Skeleton Warrior");
			break;
	}

	EnemyInfo.Level = Level;
	if (UHASAttributeSet* HASAS = Cast<UHASAttributeSet>(AttributeSetComp))
	{
		float Health = HASAS->GetHealth();
		float MaxHealth = HASAS->GetMaxHealth();
		EnemyInfo.HealthRatio = Health / MaxHealth;
	}

	return EnemyInfo;
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
	HASAIController->GetBlackboardComponent()->SetValueAsObject(FName("ChaseTarget"), nullptr);
	HASAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DistanceToTarget"), 0.f);
	HASAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), false);
	HASAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHit"), false);
	HASAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsTargetDead"), false);
	HASAIController->GetBlackboardComponent()->SetValueAsVector(FName("PatrolLocation"), FVector::ZeroVector);
}

void AHASEnemy::BeginPlay()
{
	Super::BeginPlay();

	FName SocketName;
	switch (CharacterClass)
	{
	case ECharacterClass::ECC_Warrior:
		SocketName = FName("SwordSocket");
		break;
	case ECharacterClass::ECC_Archer:
		SocketName = FName("BowSocket");
		break;
	case ECharacterClass::ECC_Mage:
		SocketName = FName("StaffSocket");
		break;
	}
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);

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
	
	for (TTuple<FGameplayTag, UNiagaraComponent*> pair : DebuffTagToNiagara)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(pair.Key, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHASCharacterBase::DebuffTagEvent);
	}

}

