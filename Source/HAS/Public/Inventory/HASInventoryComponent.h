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
	TArray<AHASItem*> Equipment;

	UPROPERTY(BlueprintReadOnly)
	TArray<AHASItem*> Potion;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAS_API UHASInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHASInventoryComponent();

	UPROPERTY(BlueprintAssignable)
	FInventoryUpdateSignature InventoryUpdate;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddItem(AHASItem* ItemToAdd);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDropItem(AHASItem* ItemToDrop);

protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(ReplicatedUsing = OnRep_Inventory, meta = (AllowPrivateAccess = true), BlueprintReadOnly)
	FAllItems Inventory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASItem> DefaultEquipmentClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASItem> DefaultPotionClass;

	UFUNCTION()
	void OnRep_Inventory();

};
