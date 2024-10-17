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
	if (ItemStruct.Quantity == 0) return;

	FDataTableRowHandle ItemHandle = ItemStruct.ItemHandle;
	if (FItemInfo* Info = ItemHandle.DataTable->FindRow<FItemInfo>(ItemHandle.RowName, ""))
	{
		if (ItemStruct.ItemType == EItemType::EIT_Equipment)
		{
			// 장착 먼저(EquipmentUse) -> EquipmentSlot에 장착한 장비가 있으면 해제 순서.
			Equipment[Index] = FItemStruct();

			// Server / Client 자신의 EquipmentSlot, Mesh, Effect Update.
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

void UHASInventoryComponent::ServerUnEquipItem_Implementation(const FItemStruct& UnEquipItemStruct, bool IsChangeEquipment)
{
	// 장착 먼저(EquipmentUse) -> 장착 해제. 
	// 다른 장비와 교체했다면 UseItem에서 이미 EquipmnetUse를 Broadcast했기 때문에 HASCharacter->ServerUseItem 호출해서 Mesh, Effect Update.
	// 따라서 다른 장비와 교체할 경우에는 UnEquipEquipment Broadcast할 필요 X
	
	// 장비만 해제했다면 UnEquipEquipment를 Broadcast해서 EquipmentMesh, PrevItemEffect 제거함.
	AddEquipment(UnEquipItemStruct);

	if(!IsChangeEquipment)
	{
		ClientUnEquipEquipment(UnEquipItemStruct);
	}
}

void UHASInventoryComponent::ClientUseEquipment_Implementation(const FItemStruct& ItemStruct)
{
	EquipmentUse.Broadcast(ItemStruct);
}

void UHASInventoryComponent::ClientUnEquipEquipment_Implementation(const FItemStruct& ItemStruct)
{
	EquipmentUnEquip.Broadcast(ItemStruct);
}

void UHASInventoryComponent::ServerChangeItem_Implementation(const FItemStruct& ItemStruct, int32 ThisItemIndex, int32 ChangeItemIndex)
{
	if (ItemStruct.Quantity == 0) return;

	if (ItemStruct.ItemType == EItemType::EIT_Equipment)
	{
		if (Equipment[ChangeItemIndex].Quantity > 0)
		{
			FItemStruct TempItemStruct = Equipment[ThisItemIndex];
			Equipment[ThisItemIndex] = Equipment[ChangeItemIndex];
			Equipment[ChangeItemIndex] = TempItemStruct;
		}
		else
		{
			Equipment[ThisItemIndex] = FItemStruct();
			Equipment[ChangeItemIndex] = ItemStruct;
		}
		EquipmentUpdate.Broadcast();
	}
	else
	{
		if (Potion[ChangeItemIndex].Quantity > 0)
		{
			FItemStruct TempItemStruct = Potion[ThisItemIndex];
			Potion[ThisItemIndex] = Potion[ChangeItemIndex];
			Potion[ChangeItemIndex] = TempItemStruct;
		}
		else
		{
			Potion[ThisItemIndex] = FItemStruct();
			Potion[ChangeItemIndex] = ItemStruct;
		}
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
		Gold = 0;

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
	if (ThisItemStruct.Quantity == 0) return;

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

		if (Potion[idx].Quantity == 0)
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

	// Default HASItem Actor를 Spawn후 ItemStruct 적용.
	if (ItemStruct.ItemType == EItemType::EIT_Equipment)
	{
		Equipment[Index] = FItemStruct();
		SpawnItem(DefaultEquipmentClass, ItemStruct);
		EquipmentUpdate.Broadcast();
	}
	else
	{
		Potion[Index] = FItemStruct();
		SpawnItem(DefaultPotionClass, ItemStruct);
		PotionUpdate.Broadcast();
	}
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
	if(DropItem) DropItem->ItemStruct = InItemStruct;
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

void UHASInventoryComponent::OnRep_Gold()
{
	GoldUpdate.Broadcast();
}

