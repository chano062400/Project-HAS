#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/HASItem.h"
#include "HASInventoryComponent.generated.h"

class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateSignature);

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
	
	//virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

private:

	UPROPERTY(ReplicatedUsing = OnRep_Equipment, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AHASItem*> Equipment;

	UPROPERTY(ReplicatedUsing = OnRep_Potion, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AHASItem*> Potion;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASItem> DefaultEquipmentClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASItem> DefaultPotionClass;

	UFUNCTION()
	void OnRep_Equipment();

	UFUNCTION()
	void OnRep_Potion();

};
