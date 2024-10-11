#include "UI/Widget/HASUserWidget.h"
#include "Net/UnrealNetwork.h"

void UHASUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
