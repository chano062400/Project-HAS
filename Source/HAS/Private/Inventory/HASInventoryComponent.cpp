#include "Inventory/HASInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actor/HASItem.h"
#include "Engine/DataTable.h"
#include "Engine/ActorChannel.h"

UHASInventoryComponent::UHASInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;	
	/**
	* When true the replication system will only replicate the registered subobjects list
	* When false the replication system will instead call the virtual ReplicateSubObjects() function where the subobjects need to be manually replicated.
	*/

}

void UHASInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		Equipment.Init(nullptr, 33);
		Potion.Init(nullptr, 33);

		//for (int idx = 0; idx < 33; idx++)
		//{
		//	// Equipment 아이템 생성 및 등록
		//	AHASItem* NewEquipment = NewObject<AHASItem>(this, DefaultEquipmentClass);
		//	if (NewEquipment)
		//	{
		//		NewEquipment->SetReplicates(true);
		//		Equipment[idx] = NewEquipment;
		//	}

		//	// Potion 아이템 생성 및 등록
		//	AHASItem* NewPotion = NewObject<AHASItem>(this, DefaultPotionClass);
		//	if (NewPotion)
		//	{
		//		NewPotion->SetReplicates(true);
		//		Potion[idx] = NewPotion;
		//	}
		//}

	}
}

void UHASInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Equipment, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Potion, COND_OwnerOnly);
}
//
//bool UHASInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
//{
//	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
//
//	for (int32 Idx = 0; Idx != Potion.Num(); Idx++)
//	{
//		AHASItem* Item = Equipment[Idx];
//		if (!IsValid(Item)) continue;
//		WroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
//	}
//
//	for (int32 Idx = 0; Idx != Potion.Num(); Idx++)
//	{
//		AHASItem* Item = Potion[Idx];
//		if (!IsValid(Item)) continue;
//		WroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
//	}
//
//	return WroteSomething;
//}

void UHASInventoryComponent::OnRep_Equipment()
{
	InventoryUpdate.Broadcast();
}

void UHASInventoryComponent::OnRep_Potion()
{
	InventoryUpdate.Broadcast();
}

void UHASInventoryComponent::ServerAddItem_Implementation(AHASItem* ItemToAdd)
{
	FDataTableRowHandle DataTableHandle = ItemToAdd->ItemStruct.ItemHandle;

	EItemType ItemType = ItemToAdd->ItemStruct.ItemType;
	int32 CanAddIdx = Equipment.Num() - 1;
	int32 AmountToAdd = ItemToAdd->ItemStruct.Quantity;

	if (ItemType == EItemType::EIT_Equipment)
	{
		for (int32 idx = 0; idx < Equipment.Num(); idx++)
		{
			if (Equipment[idx] == nullptr)
			{
				CanAddIdx = FMath::Min(CanAddIdx, idx);
			}
		}
		Equipment[CanAddIdx] = ItemToAdd;
		Equipment[CanAddIdx]->ItemStruct.Quantity = ItemToAdd->ItemStruct.Quantity;
		InventoryUpdate.Broadcast();
	}
	else if (ItemType == EItemType::EIT_Potion)
	{
		for (int32 idx = 0; idx < Equipment.Num(); idx++)
		{
			AHASItem* CurItem = Potion[idx];
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
		Potion[CanAddIdx] = ItemToAdd;
		InventoryUpdate.Broadcast();
	}

	ItemToAdd->Destroy();
}

void UHASInventoryComponent::ServerDropItem_Implementation(AHASItem* ItemToDrop)
{
	FTransform DropTransform = FTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation(), FVector(1.f, 1.f, 1.f));

	if (ItemToDrop->ItemStruct.ItemType == EItemType::EIT_Equipment)
	{
		for (int idx = 0; idx < Equipment.Num(); idx++)
		{
			AHASItem* CurItem = Equipment[idx];
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

