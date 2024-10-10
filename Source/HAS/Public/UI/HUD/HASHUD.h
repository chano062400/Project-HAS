#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HASHUD.generated.h"

class UHASUserWidget;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UInventoryWidgetController;

/**
 * 
 */
UCLASS()
class HAS_API AHASHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void InitAttributeMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void InitSpellMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void InitInventory(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
	
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);

	/* Overlay Widget */

	UPROPERTY(EditDefaultsOnly, Category = "Widget|Overlay")
	TSubclassOf<UHASUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UHASUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget|Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	/* AttributeMenu Widget*/

	UPROPERTY(EditDefaultsOnly, Category = "Widget|AttributeMenu")
	TSubclassOf<UHASUserWidget> AttributeMenuWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UHASUserWidget> AttributeMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget|AttributeMenu")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	/* SpellMenu Widget*/

	UPROPERTY(EditDefaultsOnly, Category = "Widget|SpellMenu")
	TSubclassOf<UHASUserWidget> SpellMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UHASUserWidget> SpellMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget|SpellMenu")
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	/* Inventory */

	UPROPERTY(EditDefaultsOnly, Category = "InventoryMenu")
	TSubclassOf<UHASUserWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UHASUserWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryMenu")
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;


};
