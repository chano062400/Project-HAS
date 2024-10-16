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
#include "Actor/HASItem.h"

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
	if (HasAuthority())
	{
		HASAIController->BehaviorTree->StopTree();
	
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(GetActorLocation().X, GetActorLocation().Y + 50.f, GetActorLocation().Z + 50.f));
		SpawnTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 0.f)));

		SpawnGold(SpawnTransform);
		SpawnPotionByChance(SpawnTransform);
		SpawnItemByChance(SpawnTransform);
	}
	Super::Die();
}

void AHASEnemy::SpawnGold(const FTransform& SpawnTransform)
{
	if (SpawnGoldClass)
	{
		AHASItem* SpawnedGold = GetWorld()->SpawnActorDeferred<AHASItem>(SpawnGoldClass,
			SpawnTransform,
			nullptr,
			this,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);
		SpawnedGold->ItemStruct.Quantity = 10 * FMath::Pow(1.1, Level);
		SpawnedGold->FinishSpawning(SpawnTransform);
	}
}

void AHASEnemy::SpawnItemByChance(const FTransform& SpawnTransform)
{
	bool bDrop = DropChance >= FMath::RandRange(0, 100);
	if (bDrop && SpawnItemClasses.Num() > 0)
	{
		int32 RandIdx = FMath::RandRange(0, SpawnItemClasses.Num() - 1);
		AHASItem* SpawnedItem = GetWorld()->SpawnActorDeferred<AHASItem>(SpawnItemClasses[RandIdx],
			SpawnTransform,
			nullptr,
			this,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);

		float RandomValue = FMath::RandRange(0.0f, 100.0f);
		EItemRarity Rarity = EItemRarity::EIT_None;
		// 확률에 따라 아이템 등급 결정
		if (RandomValue <= 60.0f)
		{
			Rarity = EItemRarity::EIR_Common;
		}
		else if (RandomValue <= 85.0f)
		{
			Rarity = EItemRarity::EIR_Rare;
		}
		else if (RandomValue <= 95.0f)
		{
			Rarity = EItemRarity::EIR_Unique;
		}
		else
		{
			Rarity = EItemRarity::EIR_Legendary;
		}

		SpawnedItem->ItemStruct.Rarity = Rarity;
		SpawnedItem->FinishSpawning(SpawnTransform);
	}
}

void AHASEnemy::SpawnPotionByChance(const FTransform& SpawnTransform)
{
	bool bDrop = DropChance >= FMath::RandRange(0, 100);
	if (bDrop && SpawnPotionClasses.Num() > 0)
	{
		int32 RandIdx = FMath::RandRange(0, SpawnPotionClasses.Num() - 1);
		AHASItem* SpawnedItem = GetWorld()->SpawnActorDeferred<AHASItem>(SpawnPotionClasses[RandIdx],
			SpawnTransform,
			nullptr,
			this,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);

		SpawnedItem->FinishSpawning(SpawnTransform);
	}
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

