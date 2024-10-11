#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "HASItem.generated.h"

class UWidgetComponent;
class UGameplayEffect;
class USphereComponent;

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Unique UMETA(DisplayName = "Unique"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),
	EIR_MAX UMETA(DisplayName = "MAX")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIT_Potion UMETA(DisplayName = "Potion"),
	EIT_Equipment UMETA(DisplayName = "Equipment"),
	EIT_Gold UMETA(DisplayName = "Gold"),
	EIT_MAX UMETA(DisplayName = "MAX")
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_None UMETA(DisplayName = "None"),
	EET_Staff UMETA(DisplayName = "Staff"),
	EET_Armor UMETA(DisplayName = "Armor"),
	EET_Shoes UMETA(DisplayName = "Shoes"),
	EET_Ring UMETA(DisplayName = "Ring"),
	EET_MAX UMETA(DisplayName = "MAX")
};

UENUM(BlueprintType)
enum class EPotionType : uint8
{
	EPT_None UMETA(DisplayName = "None"),
	EPT_Health UMETA(DisplayName = "Health"),
	EPT_Mana UMETA(DisplayName = "Mana"),
	EPT_MAX UMETA(DisplayName = "MAX")
};

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

	bool operator==(const FItemStruct& InItemStruct)
	{
		if (ItemType != InItemStruct.ItemType) return false;
		if (EquipeMentType != InItemStruct.EquipeMentType) return false;
		if (PotionType != InItemStruct.PotionType) return false;
		if (Rarity != InItemStruct.Rarity) return false;
		if (Quantity != InItemStruct.Quantity) return false;
		return true;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle ItemHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType = EItemType::EIT_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEquipmentType EquipeMentType = EEquipmentType::EET_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemRarity Rarity = EItemRarity::EIT_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPotionType PotionType = EPotionType::EPT_None;
};

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Power;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Price;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UGameplayEffect> UseEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UGameplayAbility> GrantedAbility;

};
UCLASS()
class HAS_API AHASItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AHASItem();

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, Category = "Item")
	FItemStruct ItemStruct;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly)
	float SpawnImpulse = 200.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> NameWidget;


};
