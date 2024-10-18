#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/HASItem.h"
#include "HASInventoryComponent.generated.h"

class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemUpdatetSignature, const FItemStruct&, ItemStruct);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class HAS_API UHASInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHASInventoryComponent();

	UPROPERTY(BlueprintAssignable)
	FInventoryUpdateSignature EquipmentUpdate;
	
	UPROPERTY(BlueprintAssignable)
	FInventoryUpdateSignature PotionUpdate;
	
	UPROPERTY(BlueprintAssignable)
	FInventoryUpdateSignature GoldUpdate;

	UPROPERTY(BlueprintAssignable)
	FItemUpdatetSignature EquipmentUse;
	
	UPROPERTY(BlueprintAssignable)
	FItemUpdatetSignature EquipmentUnEquip;

	UPROPERTY(BlueprintAssignable)
	FItemUpdatetSignature PotionUse;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddItem(AHASItem* ItemToAdd);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDropItem(const FItemStruct& ItemStruct, int32 Index);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUseItem(const FItemStruct& ItemStruct, int32 Index);
	
	UFUNCTION(Client, Reliable)
	void ClientUseEquipment(const FItemStruct& ItemStruct);
	
	UFUNCTION(Client, Reliable)
	void ClientUsePotion(const FItemStruct& ItemStruct);
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateEquipment();
	
	UFUNCTION(Client, Reliable)
	void ClientUpdatePotion();
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateGold();
	
	UFUNCTION(Client, Reliable)
	void ClientUnEquipEquipment(const FItemStruct& ItemStruct);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUnEquipItem(const FItemStruct& UnEquipItemStruct, bool IsChangeEquipment);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerChangeItem(const FItemStruct& ItemStruct, int32 ThisItemIndex, int32 ChangeItemIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSortByItemType(EItemType ItemType, bool IsGreater);

	UPROPERTY(ReplicatedUsing = OnRep_Equipment, BlueprintReadOnly)
	TArray<FItemStruct> Equipment;

	UPROPERTY(ReplicatedUsing = OnRep_Potion, BlueprintReadOnly)
	TArray<FItemStruct> Potion;

	UPROPERTY(ReplicatedUsing = OnRep_Gold, BlueprintReadOnly)
	int32 Gold = 0;

protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SpawnItem(TSubclassOf<AHASItem> ItemClass, const FItemStruct& InItemStruct);

private:

	void AddEquipment(const FItemStruct& ThisItemStruct);

	void AddPotion(FItemStruct& ThisItemStruct);
	
	void AddGold(const FItemStruct& ThisItemStruct);

	UFUNCTION()
	void OnRep_Equipment();

	UFUNCTION()
	void OnRep_Potion();

	UFUNCTION()
	void OnRep_Gold();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASItem> DefaultEquipmentClass;
		
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASItem> DefaultPotionClass;

};
