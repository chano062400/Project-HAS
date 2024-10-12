#include "UI/Widget/HASUserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Character/HASCharacter.h"
#include "Actor/HASItem.h"
#include "Inventory/HASInventoryComponent.h"
#include "UI/WidgetController/HASWidgetController.h"

void UHASUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
