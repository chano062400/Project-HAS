#include "Inventory/HASInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actor/HASItem.h"
#include "Engine/DataTable.h"

UHASInventoryComponent::UHASInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Inventory.Equipment.SetNum(33);
	Inventory.Potion.SetNum(33);

	
}

void UHASInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (int idx = 0; idx < 33; idx++)
	{
		AHASItem* NewEquipment = NewObject<AHASItem>(this, DefaultEquipmentClass);
		Inventory.Equipment[idx] = NewEquipment;
		AHASItem* NewPotion = NewObject<AHASItem>(this, DefaultPotionClass);
		Inventory.Potion[idx] = NewPotion;
	}
}

void UHASInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Inventory, COND_OwnerOnly);
}

void UHASInventoryComponent::OnRep_Inventory()
{
	InventoryUpdate.Broadcast();
}

void UHASInventoryComponent::ServerAddItem_Implementation(AHASItem* ItemToAdd)
{
	FDataTableRowHandle DataTableHandle = ItemToAdd->ItemStruct.ItemHandle;

	EItemType ItemType = ItemToAdd->ItemStruct.ItemType;
	int32 CanAddIdx = Inventory.Equipment.Num() - 1;
	int32 AmountToAdd = ItemToAdd->ItemStruct.Quantity;

	if (ItemType == EItemType::EIT_Equipment)
	{
		for (int32 idx = 0; idx < Inventory.Equipment.Num(); idx++)
		{
			auto& CurItem = Inventory.Equipment[idx];
			if (CurItem->ItemStruct.EquipeMentType == EEquipmentType::EET_None)
			{
				CanAddIdx = FMath::Min(CanAddIdx, idx);
			}
		}
		Inventory.Equipment[CanAddIdx] = ItemToAdd;
		Inventory.Equipment[CanAddIdx]->ItemStruct.Quantity = ItemToAdd->ItemStruct.Quantity;
		InventoryUpdate.Broadcast();
	}
	else if (ItemType == EItemType::EIT_Potion)
	{
		for (int32 idx = 0; idx < Inventory.Equipment.Num(); idx++)
		{
			AHASItem* CurItem = Inventory.Potion[idx];
			if (CurItem->ItemStruct.PotionType == EPotionType::EPT_None)
			{
				CanAddIdx = FMath::Min(CanAddIdx, idx);
			}

			// 같은 아이템이 있다면
			if (DataTableHandle.RowName == CurItem->ItemStruct.ItemHandle.RowName)
			{
				FItemInfo* ItemInfo = DataTableHandle.DataTable->FindRow<FItemInfo>(DataTableHandle.RowName, "");

				int32 EmptyQuantity = ItemInfo->MaxStackSize - CurItem->ItemStruct.Quantity;
				CurItem->ItemStruct.Quantity = FMath::Min(EmptyQuantity, ItemToAdd->ItemStruct.Quantity);
				AmountToAdd = ItemToAdd->ItemStruct.Quantity - EmptyQuantity;
			}
		}
		ItemToAdd->ItemStruct.Quantity = AmountToAdd;
		Inventory.Potion[CanAddIdx] = ItemToAdd;
		InventoryUpdate.Broadcast();
	}

	ItemToAdd->Destroy();
}

void UHASInventoryComponent::ServerDropItem_Implementation(AHASItem* ItemToDrop)
{
	FTransform DropTransform = FTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation(), FVector(1.f, 1.f, 1.f));

	if (ItemToDrop->ItemStruct.ItemType == EItemType::EIT_Equipment)
	{
		for (int idx = 0; idx < Inventory.Equipment.Num(); idx++)
		{
			AHASItem* CurItem = Inventory.Equipment[idx];
			if (ItemToDrop->ItemStruct == CurItem->ItemStruct)
			{
				CurItem->ItemStruct = FItemStruct();
				AHASItem* DropItem = GetWorld()->SpawnActorDeferred<AHASItem>(
					AHASItem::StaticClass(),
					DropTransform,
					nullptr,
					nullptr,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
				);
				DropItem->ItemStruct = ItemToDrop->ItemStruct;
				DropItem->FinishSpawning(DropTransform);
			}
		}
	}
	else
	{
	
	}
}

