#include "UI/WidgetController/HASWidgetController.h"

void UHASWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PC = WCParams.PC;
	PS = WCParams.PS;
	ASC = WCParams.ASC;
	AS = WCParams.AS;
}
