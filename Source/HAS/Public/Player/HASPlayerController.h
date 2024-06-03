#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "Interfaces/HASCombatInterface.h"
#include "HASPlayerController.generated.h"

class UInputMappingContext;
class UHASEnhancedInputComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class HAS_API AHASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AHASPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

protected:

	virtual void BeginPlay() override;

private:

	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void OpenAttributeMenu();

	bool bOpenedAttributeMenu = false;

	void AbilityInputTagPressed(FGameplayTag InputTag);

	void AbilityInputTagReleased(FGameplayTag InputTag);
	
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void CursorTrace();

	void AutoRun();

	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputMappingContext> HASMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> AttributeMenuAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UHASInputInfo> InputInfo;

	FHitResult MouseCursorHitResult;

	IHASCombatInterface* ThisActor;
	
	IHASCombatInterface* LastActor;

	UPROPERTY(VisibleAnywhere, Category = "Combat|Cursor")
	bool bClicked = false;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Cursor")
	TObjectPtr<UUserWidget> DefaultMouseCursorWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Cursor")
	TObjectPtr<UUserWidget> HighlightMouseCursorWidget;

	UPROPERTY(VisibleAnywhere, Category = "Combat|Movement")
	FVector CachedDestination = FVector();

	UPROPERTY(VisibleAnywhere, Category = "Combat|Movement")
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(VisibleAnywhere, Category = "Combat|Movement")
	bool bTargeting = false;

	UPROPERTY(VisibleAnywhere, Category = "Combat|Movement")
	bool bAutoRun = false;

	UPROPERTY(VisibleAnywhere, Category = "Combat|Movement")
	float ElapsedTime = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat|Movement")
	float ShortPressThreshold = 0.5f;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat|Movement")
	float AutoRunAcceptance = 50.f;
};
