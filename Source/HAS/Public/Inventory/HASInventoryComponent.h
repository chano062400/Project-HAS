#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/HASItem.h"
#include "HASInventoryComponent.generated.h"

class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FItemUsetSignature, const FItemStruct& ItemStruct);

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

	FItemUsetSignature EquipmentUse;

	FItemUsetSignature PotionUse;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddItem(AHASItem* ItemToAdd);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDropItem(const FItemStruct& ItemStruct, int32 Index);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUseItem(const FItemStruct& ItemStruct, int32 Index);

	UPROPERTY(ReplicatedUsing = OnRep_Equipment, BlueprintReadOnly)
	TArray<FItemStruct> Equipment;

	UPROPERTY(ReplicatedUsing = OnRep_Potion, BlueprintReadOnly)
	TArray<FItemStruct> Potion;

protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SpawnItem(TSubclassOf<AHASItem> ItemClass, const FItemStruct& InItemStruct);

private:

	UFUNCTION()
	void OnRep_Equipment();

	UFUNCTION()
	void OnRep_Potion();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASItem> EquipmentClass;
		
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASItem> PotionClass;

};
