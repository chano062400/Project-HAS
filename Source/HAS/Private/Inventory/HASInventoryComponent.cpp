#include "Inventory/HASInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actor/HASItem.h"
#include "Engine/DataTable.h"
#include "Engine/ActorChannel.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/HASCharacter.h"
#include "Components/SceneCaptureComponent2D.h"

UHASInventoryComponent::UHASInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;	
}

void UHASInventoryComponent::ServerUseItem_Implementation(const FItemStruct& ItemStruct, int32 Index)
{
	FDataTableRowHandle ItemHandle = ItemStruct.ItemHandle;
	FItemInfo* Info = ItemHandle.DataTable->FindRow<FItemInfo>(ItemHandle.RowName, "");

	for (const auto& Effect : Info->UseEffects)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
		{
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, 1.f, ContextHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), ASC->GetPredictionKeyForNewAction());
		}
	}

	if (ItemStruct.ItemType == EItemType::EIT_Equipment)
	{
		if(AHASCharacter* Player = Cast<AHASCharacter>(GetOwner()))
		{
			Player->GetWeapon()->SetSkeletalMesh(Info->StaffMesh);
			Player->GetSceneCaptureComponent2D()->CaptureScene();
		}
		Equipment[Index] = FItemStruct();
		EquipmentUpdate.Broadcast();
	}
	else
	{
		Potion[Index].Quantity -= 1;
		if (Potion[Index].Quantity == 0) Potion[Index] = FItemStruct();
		PotionUpdate.Broadcast();
	}
}

void UHASInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->HasAuthority())
	{
		FItemStruct ItemStruct;
		Equipment.Init(ItemStruct, 33);
		Potion.Init(ItemStruct, 33);
	}
}

void UHASInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Equipment, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Potion, COND_OwnerOnly);
}

void UHASInventoryComponent::SpawnItem(TSubclassOf<AHASItem> ItemClass, const FItemStruct& InItemStruct)
{
	FTransform DropTransform = FTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation(), FVector(1.f, 1.f, 1.f));

	AHASItem* DropItem = GetWorld()->SpawnActorDeferred<AHASItem>(
		ItemClass,
		DropTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	DropItem->ItemStruct = InItemStruct;
	DropItem->FinishSpawning(DropTransform);
}

void UHASInventoryComponent::OnRep_Equipment()
{
	EquipmentUpdate.Broadcast();
}

void UHASInventoryComponent::OnRep_Potion()
{
	PotionUpdate.Broadcast();
}

void UHASInventoryComponent::ServerAddItem_Implementation(AHASItem* ItemToAdd)
{
	int32 CanAddIdx;
	int32 AmountToAdd = ItemToAdd->ItemStruct.Quantity;

	if (ItemToAdd->ItemStruct.ItemType == EItemType::EIT_Equipment)
	{
		CanAddIdx = Equipment.Num();
		for (int32 idx = 0; idx < Equipment.Num(); idx++)
		{
			if (Equipment[idx].Quantity == 0)
			{
				CanAddIdx = FMath::Min(CanAddIdx, idx);
			}
		}

		Equipment[CanAddIdx] = ItemToAdd->ItemStruct;
		EquipmentUpdate.Broadcast();
	}
	else if (ItemToAdd->ItemStruct.ItemType == EItemType::EIT_Potion)
	{
		CanAddIdx = Potion.Num();
		for (int32 idx = 0; idx < Potion.Num(); idx++)
		{
			
			if (Potion[idx].PotionType == EPotionType::EPT_None)
			{
				CanAddIdx = FMath::Min(CanAddIdx, idx);
			}

			// 같은 아이템이 있다면
			if (ItemToAdd->ItemStruct.ItemHandle.RowName == Potion[idx].ItemHandle.RowName)
			{
				FItemInfo* ItemInfo = ItemToAdd->ItemStruct.ItemHandle.DataTable->FindRow<FItemInfo>(ItemToAdd->ItemStruct.ItemHandle.RowName, "");

				int32 CanAddAmount = ItemInfo->MaxStackSize - Potion[idx].Quantity;
				Potion[idx].Quantity = FMath::Min(ItemInfo->MaxStackSize, Potion[idx].Quantity + ItemToAdd->ItemStruct.Quantity);		
				AmountToAdd = ItemToAdd->ItemStruct.Quantity  - CanAddAmount;
			}
		}
		if (AmountToAdd > 0)
		{
			ItemToAdd->ItemStruct.Quantity = AmountToAdd;
			Potion[CanAddIdx] = ItemToAdd->ItemStruct;
		}
		PotionUpdate.Broadcast();
	}

	ItemToAdd->Destroy();
}

void UHASInventoryComponent::ServerDropItem_Implementation(const FItemStruct& ItemStruct, int32 Index)
{
	if (ItemStruct.Quantity == 0) return;

	if (ItemStruct.ItemType == EItemType::EIT_Equipment)
	{
		Equipment[Index] = FItemStruct();
		SpawnItem(EquipmentClass, ItemStruct);
		EquipmentUpdate.Broadcast();
	}
	else
	{
		Potion[Index] = FItemStruct();
		SpawnItem(PotionClass, ItemStruct);
		PotionUpdate.Broadcast();
	}
}

