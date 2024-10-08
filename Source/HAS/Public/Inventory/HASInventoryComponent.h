#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/HASItem.h"
#include "HASInventoryComponent.generated.h"

class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateSignature);

USTRUCT(BlueprintType)
struct FAllItems
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FItemStruct> Equipment;

	UPROPERTY(BlueprintReadOnly)
	TArray<FItemStruct> Potion;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAS_API UHASInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHASInventoryComponent();

	UPROPERTY(BlueprintAssignable)
	FInventoryUpdateSignature InventoryUpdate;

	UFUNCTION(Server, Reliable)
	void ServerAddItem(AHASItem* ItemToAdd);

	UFUNCTION(Server, Reliable)
	void ServerRemoveItem(AHASItem* ItemToRemove);
	
	UFUNCTION(Server, Reliable)
	void ServerUpdateInventory();
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateInventory();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventory();

protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(ReplicatedUsing = OnRep_Inventory, meta = (AllowPrivateAccess = true), BlueprintReadOnly)
	FAllItems Inventory;

	UFUNCTION()
	void OnRep_Inventory();

};
