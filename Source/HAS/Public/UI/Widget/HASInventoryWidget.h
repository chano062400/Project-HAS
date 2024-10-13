#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/HASUserWidget.h"
#include "Actor/HASItem.h"
#include "HASInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASInventoryWidget : public UHASUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "Inventory")
	FItemStruct ThisItemStruct;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "Inventory")
	TObjectPtr<AHASItem> ThisItem;

	UFUNCTION(BlueprintCallable)
	FString LineBreak(FString Text);

	UFUNCTION(BlueprintCallable)
	FString GetInformationDescription(const FItemStruct& ItemStruct);

	FString GetRarityAsString(EItemRarity Rarity);
};
