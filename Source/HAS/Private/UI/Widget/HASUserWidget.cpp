#include "UI/Widget/HASUserWidget.h"

void UHASUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
