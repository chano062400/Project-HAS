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
	Hat->SetupAttachment(GetMesh(), FName("HatSocket"));

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("HairGroom"));
	Hair->SetupAttachment(GetMesh(), FName("HairGroom"));

	LevelUpEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUp Effect"));
	LevelUpEffectComponent->SetupAttachment(GetRootComponent());
	LevelUpEffectComponent->bAutoActivate = false;

	Inventory = CreateDefaultSubobject<UHASInventoryComponent>(TEXT("Inventory"));
	Inventory->SetIsReplicated(true);
}

void AHASCharacter::BeginPlay()
{
	Super::BeginPlay();

	Inventory->EquipmentUse.AddDynamic(this, &AHASCharacter::ServerEquipmentUse);
	Inventory->EquipmentUnEquip.AddDynamic(this, &AHASCharacter::ServerEquipmentUnEquip);
	Inventory->PotionUse.AddDynamic(this, &AHASCharacter::ServerPotionUse);

}

void AHASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	AddStartAbilities();

	AbilitySystemComponent->RegisterGameplayTagEvent(FHASGameplayTags::Get().Potion_Critical, EGameplayTagEventType::NewOrRemoved).AddLambda(
		[this](const FGameplayTag Tag, int32 NewCount)
		{
			if (NewCount > 0) 
			{
				PotionApplied.Broadcast(Tag);
			}
		}
	);
	
	AbilitySystemComponent->RegisterGameplayTagEvent(FHASGameplayTags::Get().Potion_Elixir, EGameplayTagEventType::NewOrRemoved).AddLambda(
		[this](const FGameplayTag Tag, int32 NewCount)
		{
			if (NewCount > 0) 
			{
				PotionApplied.Broadcast(Tag);
			}
		}
	);

}

void AHASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();

	AbilitySystemComponent->RegisterGameplayTagEvent(FHASGameplayTags::Get().Potion_Critical, EGameplayTagEventType::NewOrRemoved).AddLambda(
		[this](const FGameplayTag Tag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				PotionApplied.Broadcast(Tag);
			}
		}
	);

	AbilitySystemComponent->RegisterGameplayTagEvent(FHASGameplayTags::Get().Potion_Elixir, EGameplayTagEventType::NewOrRemoved).AddLambda(
		[this](const FGameplayTag Tag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				PotionApplied.Broadcast(Tag);
			}
		}
	);
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

void AHASCharacter::ServerEquipmentUse_Implementation(const FItemStruct& ItemStruct)
{
	FDataTableRowHandle ItemHandle = ItemStruct.ItemHandle;
	if (FItemInfo* Info = ItemHandle.DataTable->FindRow<FItemInfo>(ItemHandle.RowName, ""))
	{
		SetEquipmentMeshByType(ItemStruct);

		// 전에 장착했던 아이템 효과 모두 제거.
		RemovePrevEquipmentEffect(ItemStruct);

		for (const auto& Effect : Info->UseEffects)
		{
			FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, ContextHandle);

			float ApplyLevel = 1;

			// 아이템 등급에 따라 Effect 레벨 설정
			SetEffectLevelByRarity(ItemStruct, ApplyLevel);

			if (SpecHandle.IsValid())
			{
				if (FGameplayEffectSpec* EffectSpec = SpecHandle.Data.Get())
				{
					EffectSpec->SetLevel(ApplyLevel);
				}
			}
			
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), AbilitySystemComponent->GetPredictionKeyForNewAction());

			// 현재 아이템 효과들 저장.
			if (EffectHandle.IsValid())
			{
				FActiveGameplayEffectHandle& PrevHandle = PrevEquipmentEffectHandle.FindOrAdd(ItemStruct.EquipmentType);
				PrevHandle = EffectHandle;
			}
		}
	}
}

void AHASCharacter::RemovePrevEquipmentEffect(const FItemStruct& ItemStruct)
{
	if (PrevEquipmentEffectHandle.Contains(ItemStruct.EquipmentType))
	{
		FActiveGameplayEffectHandle& EffectHandle = PrevEquipmentEffectHandle[ItemStruct.EquipmentType];

		if (EffectHandle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(PrevEquipmentEffectHandle[ItemStruct.EquipmentType]);
			PrevEquipmentEffectHandle.Remove(ItemStruct.EquipmentType);
		}
	}
}

void AHASCharacter::RemovePrevPotionEffect(const FItemStruct& ItemStruct)
{
	if (PrevPotionEffectHandle.Contains(ItemStruct.PotionType))
	{
		FActiveGameplayEffectHandle& EffectHandle = PrevPotionEffectHandle[ItemStruct.PotionType];

		if (EffectHandle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(PrevPotionEffectHandle[ItemStruct.PotionType]);
			PrevPotionEffectHandle.Remove(ItemStruct.PotionType);
		}
	}
}

void AHASCharacter::SetEffectLevelByRarity(const FItemStruct& ItemStruct, float& ApplyLevel)
{
	switch (ItemStruct.Rarity)
	{
	case EItemRarity::EIR_Common:
		ApplyLevel = 1.0f;
		break;
	case EItemRarity::EIR_Rare:
		ApplyLevel = 1.5f;
		break;
	case EItemRarity::EIR_Unique:
		ApplyLevel = 2.0f;
		break;
	case EItemRarity::EIR_Legendary:
		ApplyLevel = 3.0f;
		break;
	default:
		break;
	}
}

void AHASCharacter::SetEquipmentMeshByType(const FItemStruct& ItemStruct)
{
	FDataTableRowHandle ItemHandle = ItemStruct.ItemHandle;
	if (FItemInfo* Info = ItemHandle.DataTable->FindRow<FItemInfo>(ItemHandle.RowName, ""))
	{
		switch (ItemStruct.EquipmentType)
		{
		case EEquipmentType::EET_Staff:
			if (Info->StaffMesh)
			{
				WeaponMesh = Info->StaffMesh;
				Weapon->SetSkeletalMesh(WeaponMesh);
			}
			break;
		case EEquipmentType::EET_Hat:
			if (Info->Mesh)
			{
				HatMesh = Info->Mesh;
				Hat->SetStaticMesh(HatMesh);
			}
			break;
		default:
			break;
		}
	}
}

void AHASCharacter::ServerPotionUse_Implementation(const FItemStruct& ItemStruct)
{
	FDataTableRowHandle ItemHandle = ItemStruct.ItemHandle;
	if (FItemInfo* Info = ItemHandle.DataTable->FindRow<FItemInfo>(ItemHandle.RowName, ""))
	{
		for (const auto& Effect : Info->UseEffects)
		{
			// Stack Limit = 1
			// Elixir, Critical 모두 효과를 1번씩만 얻을 수 있음. 더 먹을 경우 적용했던 Effect 제거하고 다시 적용하여 지속시간만 초기화.
			RemovePrevPotionEffect(ItemStruct);

			FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, ContextHandle);

			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), AbilitySystemComponent->GetPredictionKeyForNewAction());

			if (EffectHandle.IsValid())
			{
				FActiveGameplayEffectHandle& PrevHandle = PrevPotionEffectHandle.FindOrAdd(ItemStruct.PotionType);
				PrevHandle = EffectHandle;
			}
		}
	}
}

void AHASCharacter::ServerEquipmentUnEquip_Implementation(const FItemStruct& ItemStruct)
{
	// 장착했던 장비 효과 제거.
	RemovePrevEquipmentEffect(ItemStruct);

	switch (ItemStruct.EquipmentType)
	{
	case EEquipmentType::EET_Staff:
		WeaponMesh = nullptr;
		Weapon->SetSkeletalMesh(nullptr);
		break;
	case EEquipmentType::EET_Hat:
		HatMesh = nullptr;
		Hat->SetStaticMesh(nullptr);
		break;
	default:
		break;
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
