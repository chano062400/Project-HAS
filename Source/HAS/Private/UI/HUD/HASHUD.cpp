#include "UI/HUD/HASHUD.h"
#include "UI/Widget/HASUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "UI/WidgetController/InventoryWidgetController.h"

UOverlayWidgetController* AHASHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallBacks();
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AHASHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallBacks();
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AHASHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallBacks();
	}
	return SpellMenuWidgetController;
}

UInventoryWidgetController* AHASHUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallBacks();
	}
	return InventoryWidgetController;
}

void AHASHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Can't find OverlayWidgetClass"));
	checkf(OverlayWidgetControllerClass, TEXT("Can't find OverlayWidgetControllerClass"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UHASUserWidget>(Widget);

	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);

	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WCParams);
	OverlayWidget->SetWidgetController(WidgetController);
	OverlayWidgetController->BroadcastInitialValues();

	InitAttributeMenu(PC, PS, ASC, AS);
	
	InitSpellMenu(PC, PS, ASC, AS);

	InitInventory(PC, PS, ASC, AS);

	Widget->AddToViewport();

}

void AHASHUD::InitAttributeMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	check(AttributeMenuWidgetClass);
	check(AttributeMenuWidgetControllerClass);

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), AttributeMenuWidgetClass);
	AttributeMenuWidget = Cast<UHASUserWidget>(Widget);

	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);

	UAttributeMenuWidgetController* WidgetController = GetAttributeMenuWidgetController(WCParams);
	
	AttributeMenuWidget->SetWidgetController(WidgetController);
	AttributeMenuWidgetController->BroadcastInitialValues();
}

void AHASHUD::InitSpellMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	check(SpellMenuWidgetClass);
	check(SpellMenuWidgetControllerClass);

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), SpellMenuWidgetClass);
	SpellMenuWidget = Cast<UHASUserWidget>(Widget);

	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);

	USpellMenuWidgetController* WidgetController = GetSpellMenuWidgetController(WCParams);
	
	SpellMenuWidget->SetWidgetController(WidgetController);
	SpellMenuWidgetController->BroadcastInitialValues();
}

void AHASHUD::InitInventory(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	check(InventoryWidgetClass);
	check(InventoryWidgetControllerClass);

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget = Cast<UHASUserWidget>(Widget);

	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);

	UInventoryWidgetController* WidgetController = GetInventoryWidgetController(WCParams);

	InventoryWidget->SetWidgetController(WidgetController);
	InventoryWidgetController->BroadcastInitialValues();
}
