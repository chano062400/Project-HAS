#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HASHUD.generated.h"

class UHASUserWidget;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class HAS_API AHASHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UHASUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UHASUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
};
