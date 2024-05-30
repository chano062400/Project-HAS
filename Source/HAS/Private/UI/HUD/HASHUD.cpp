#include "UI/HUD/HASHUD.h"
#include "UI/Widget/HASUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

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
