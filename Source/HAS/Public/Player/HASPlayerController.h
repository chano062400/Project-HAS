#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "HASPlayerController.generated.h"

class UInputMappingContext;
class UHASEnhancedInputComponent;

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

	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputMappingContext> HASMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> AttributeMenuAction;

	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UHASInputInfo> InputInfo;
};
