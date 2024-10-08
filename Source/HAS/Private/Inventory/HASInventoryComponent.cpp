#include "Inventory/HASInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actor/HASItem.h"
#include "Engine/DataTable.h"

UHASInventoryComponent::UHASInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	FItemStruct ItemStruct;
	Inventory.Equipment.Init(ItemStruct, 33);
	Inventory.Potion.Init(ItemStruct, 33);
}

void UHASInventoryComponent::ServerUpdateInventory_Implementation()
{
	//ClientUpdateInventory();
}

void UHASInventoryComponent::ClientUpdateInventory_Implementation()
{
	//InventoryUpdate.Broadcast();
}

void UHASInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHASInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Inventory, COND_OwnerOnly);
}

void UHASInventoryComponent::OnRep_Inventory()
{
	//InventoryUpdate.Broadcast();
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
			auto& CurItemStruct = Inventory.Equipment[idx];
			if (CurItemStruct.ItemType == EItemType::EIT_None)
			{
				CanAddIdx = FMath::Min(CanAddIdx, idx);
			}
		}
		Inventory.Equipment[CanAddIdx] = ItemToAdd->ItemStruct;
		OnRep_Inventory();
	}
	else if (ItemType == EItemType::EIT_Potion)
	{
		for (int32 idx = 0; idx < Inventory.Equipment.Num(); idx++)
		{
			auto& CurItemStruct = Inventory.Equipment[idx];
			if (CurItemStruct.ItemType == EItemType::EIT_None)
			{
				CanAddIdx = FMath::Min(CanAddIdx, idx);
			}

			// 같은 아이템이 있다면
			if (DataTableHandle.RowName == CurItemStruct.ItemHandle.RowName)
			{
				FItemInfo* ItemInfo = DataTableHandle.DataTable->FindRow<FItemInfo>(DataTableHandle.RowName, "");

				int32 EmptyQuantity = ItemInfo->MaxStackSize - CurItemStruct.Quantity;
				CurItemStruct.Quantity = FMath::Min(EmptyQuantity, ItemToAdd->ItemStruct.Quantity);
				AmountToAdd = ItemToAdd->ItemStruct.Quantity - EmptyQuantity;
			}
		}
		ItemToAdd->ItemStruct.Quantity = AmountToAdd;
		Inventory.Potion[CanAddIdx] = ItemToAdd->ItemStruct;
		OnRep_Inventory();
	}

	ItemToAdd->Destroy();
}

void UHASInventoryComponent::ServerRemoveItem_Implementation(AHASItem* ItemToRemove)
{

}

