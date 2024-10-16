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
	if (FItemInfo* Info = ItemHandle.DataTable->FindRow<FItemInfo>(ItemHandle.RowName, ""))
	{
		if (ItemStruct.ItemType == EItemType::EIT_Equipment)
		{
			Equipment[Index] = FItemStruct();

			// Server / Client 자신의 EquipmentSlot Update.
			ClientUseEquipment(ItemStruct);

			EquipmentUpdate.Broadcast();

		}
		else
		{
			Potion[Index].Quantity -= 1;
			if (Potion[Index].Quantity == 0) Potion[Index] = FItemStruct();
			PotionUse.Broadcast(ItemStruct);
			PotionUpdate.Broadcast();
		}
	}
}

void UHASInventoryComponent::ServerUnEquipItem_Implementation(const FItemStruct& ChangeItemStruct)
{
	if (ChangeItemStruct.ItemType == EItemType::EIT_Equipment)
	{
		AddEquipment(ChangeItemStruct);
	}
}

void UHASInventoryComponent::ClientUseEquipment_Implementation(const FItemStruct& ItemStruct)
{
	EquipmentUse.Broadcast(ItemStruct);
}

void UHASInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->HasAuthority())
	{
		FItemStruct ItemStruct;
		Equipment.Init(ItemStruct, 33);
		Potion.Init(ItemStruct, 33);

		EquipmentUpdate.Broadcast();
		PotionUpdate.Broadcast();
		GoldUpdate.Broadcast();
	}
}

void UHASInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Equipment, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Potion, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Gold, COND_OwnerOnly);
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

void UHASInventoryComponent::ServerAddItem_Implementation(AHASItem* ItemToAdd)
{
	switch (ItemToAdd->ItemStruct.ItemType)
	{
		case EItemType::EIT_Equipment:
			AddEquipment(ItemToAdd->ItemStruct);
			break;
		case EItemType::EIT_Potion:
			AddPotion(ItemToAdd->ItemStruct);
			break;
		case EItemType::EIT_Gold:
			AddGold(ItemToAdd->ItemStruct);
			break;
		default:
			break;
	}

	ItemToAdd->Destroy();
}

void UHASInventoryComponent::AddEquipment(const FItemStruct& ThisItemStruct)
{
	int32 CanAddIdx = Equipment.Num();
	for (int32 idx = 0; idx < Equipment.Num(); idx++)
	{
		if (Equipment[idx].Quantity == 0)
		{
			CanAddIdx = FMath::Min(CanAddIdx, idx);
		}
	}

	Equipment[CanAddIdx] = ThisItemStruct;
	EquipmentUpdate.Broadcast();
}

void UHASInventoryComponent::AddPotion(FItemStruct& ThisItemStruct)
{
	int32 CanAddIdx = Potion.Num();
	int32 AmountToAdd = ThisItemStruct.Quantity;

	for (int32 idx = 0; idx < Potion.Num(); idx++)
	{

		if (Potion[idx].PotionType == EPotionType::EPT_None)
		{
			CanAddIdx = FMath::Min(CanAddIdx, idx);
		}

		// 같은 아이템이 있다면
		if (ThisItemStruct.ItemHandle.RowName == Potion[idx].ItemHandle.RowName)
		{
			FItemInfo* ItemInfo = ThisItemStruct.ItemHandle.DataTable->FindRow<FItemInfo>(ThisItemStruct.ItemHandle.RowName, "");

			int32 CanAddAmount = ItemInfo->MaxStackSize - Potion[idx].Quantity;
			Potion[idx].Quantity = FMath::Min(ItemInfo->MaxStackSize, Potion[idx].Quantity + ThisItemStruct.Quantity);
			AmountToAdd = ThisItemStruct.Quantity - CanAddAmount;
		}
	}
	if (AmountToAdd > 0)
	{
		ThisItemStruct.Quantity = AmountToAdd;
		Potion[CanAddIdx] = ThisItemStruct;
	}
	PotionUpdate.Broadcast();
}

void UHASInventoryComponent::AddGold(const FItemStruct& ThisItemStruct)
{
	int32 AmountToAdd = ThisItemStruct.Quantity;
	Gold += AmountToAdd;
	GoldUpdate.Broadcast();
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

void UHASInventoryComponent::OnRep_Equipment()
{
	EquipmentUpdate.Broadcast();
}

void UHASInventoryComponent::OnRep_Potion()
{
	PotionUpdate.Broadcast();
}

void UHASInventoryComponent::OnRep_Gold()
{
	GoldUpdate.Broadcast();
}

