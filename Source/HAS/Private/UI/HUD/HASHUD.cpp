#include "UI/HUD/HASHUD.h"
#include "UI/Widget/HASUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

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

void AHASHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Can't find OverlayWidgetClass"));
	checkf(OverlayWidgetControllerClass, TEXT("Can't find OverlayWidgetControllerClass"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UHASUserWidget>(Widget);

	FWidgetControllerParams WCParams;
	WCParams.PC = PC;
	WCParams.PS = PS;
	WCParams.ASC = ASC;
	WCParams.AS = AS;

	OverlayWidgetController = GetOverlayWidgetController(WCParams);
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
