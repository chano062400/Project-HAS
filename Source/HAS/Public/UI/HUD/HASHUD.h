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

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UHASUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UHASUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

};
