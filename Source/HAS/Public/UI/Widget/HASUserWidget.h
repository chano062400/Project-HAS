#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Actor/HASItem.h"
#include "HASUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API UHASUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FItemStruct ThisItemStruct;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "Inventory")
	TObjectPtr<AHASItem> ThisItem;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateInventory();

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

};
