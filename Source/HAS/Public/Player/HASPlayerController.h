#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "Actor/HASItem.h"
#include "Interfaces/HASCombatInterface.h"
#include "Interfaces/HASPlayerInterface.h"
#include "HASPlayerController.generated.h"

class UInputMappingContext;
class UHASEnhancedInputComponent;
class USplineComponent;
class UDamageTextComponent;
class AHASMagicCircle;
class UHASInputInfo;
class UHASAbilityInfo;
class AHASCharacter;
class AHASCharacterBase;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FEnemyInfoSignature, FEnemyInfo, /*ThisActor*/ int32 /*PlayerLevel*/);


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

	UFUNCTION(Client, Reliable)
	void ClientShowFloatingDamageText(float Damage, AActor* TargetActor, bool bIsCritical);
	
	UFUNCTION(Client, Reliable)
	void ClientShowFloatingImmnueText(AActor* TargetActor);

	UFUNCTION(Client, Reliable)
	void ClientShowEnemyInfo(FEnemyInfo Info, int32 PlayerLevel);

	UFUNCTION(BlueprintCallable)
	AHASMagicCircle* ShowMagicCircle(UMaterialInterface* DecalMaterial);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	void UpdateMagicCircle();

	FEnemyInfoSignature EnemyInfo;

protected:

	virtual void BeginPlay() override;

private:

	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void OpenAttributeMenu();
	
	void OpenSpellMenu();
	
	void OpenInventory();

	void AbilityInputTagPressed(FGameplayTag InputTag);

	void AbilityInputTagReleased(FGameplayTag InputTag);
	
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void CursorTrace();

	void AutoRun();

	void ShiftPressed();

	void ShiftReleased();

	bool bOpenedAttributeMenu = false;

	bool bOpenedSpellMenu = false;
	
	bool bOpenedInventory = false;

	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputMappingContext> HASMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> AttributeMenuAction;
		
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> SpellMenuAction;
		
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> InventoryAction;
		
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> ShiftPressedAction;
		
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UInputAction> ShiftReleasedAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "input")
	TObjectPtr<UHASInputInfo> InputInfo;

	FHitResult MouseCursorHitResult;

	IHASCombatInterface* ThisActor;
	
	IHASCombatInterface* LastActor;

	UPROPERTY(VisibleAnywhere, Category = "Combat|Cursor")
	bool bClicked = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat|Cursor")
	bool bShiftPressed = false;

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

	UPROPERTY(EditDefaultsOnly, Category ="Combat|Text")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
	UPROPERTY(EditDefaultsOnly, Category ="Combat|Text")
	TSubclassOf<UWidgetComponent> ImmuneTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHASMagicCircle> MagicCircleDecalComponent;

	UPROPERTY()
	TObjectPtr<AHASMagicCircle> MagicCircle;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickEffect;

};
