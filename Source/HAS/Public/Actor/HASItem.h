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
	EIT_Staff UMETA(DisplayName = "Staff"),
	EIT_Shoes UMETA(DisplayName = "Shoes"),
	EIT_Gold UMETA(DisplayName = "Gold"),
	EIT_MAX UMETA(DisplayName = "MAX")
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle ItemHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemRarity Rarity = EItemRarity::EIT_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType = EItemType::EIT_None;
	
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
	int32 StackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Power;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;
};
UCLASS()
class HAS_API AHASItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AHASItem();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddToInventory(const FItemStruct& InItemStruct);

	UPROPERTY(EditAnywhere, Category = "Item")
	FItemStruct ItemStruct;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


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
