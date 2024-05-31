#include "Player/HASPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/HUD/HASHUD.h"
#include "UI/Widget/HASUserWidget.h"
#include "Input/HASEnhancedInputComponent.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AHASPlayerController::AHASPlayerController()
{
	bReplicates = true;
}

void AHASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(HASMappingContext, 0);
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void AHASPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Forward = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(Forward, MoveVector.X);
		ControlledPawn->AddMovementInput(Right, MoveVector.Y);
	}
}

void AHASPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void AHASPlayerController::OpenAttributeMenu()
{
	if (AHASHUD* HASHUD = GetHUD<AHASHUD>())
	{
		check(HASHUD->AttributeMenuWidget);

		if (!bOpenedAttributeMenu)
		{
			bOpenedAttributeMenu = true;

			if (UHASUserWidget* Widget = HASHUD->AttributeMenuWidget)
			{
				FVector2D Size = Widget->GetDesiredSize();
				Widget->SetDesiredSizeInViewport(Size);
				
				int x = 0, y = 0;
				GetViewportSize(x, y);
				Widget->SetPositionInViewport(FVector2D(x / 2, 30.f));
				
				Widget->AddToViewport();
			}
		}
		else
		{
			bOpenedAttributeMenu = false;
			HASHUD->AttributeMenuWidget->RemoveFromParent();
		}
	}
}

void AHASPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn())))
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void AHASPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn())))
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void AHASPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn())))
	{
		ASC->AbilityInputTagHeld(InputTag);
	}
}

void AHASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputInfo);

	if (UHASEnhancedInputComponent* HASEnhancedInputComponent = Cast<UHASEnhancedInputComponent>(InputComponent))
	{
		HASEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHASPlayerController::Move);
		HASEnhancedInputComponent->BindAction(AttributeMenuAction, ETriggerEvent::Triggered, this, &AHASPlayerController::OpenAttributeMenu);
		HASEnhancedInputComponent->BindAbilityAction(InputInfo, this, &AHASPlayerController::AbilityInputTagPressed, &AHASPlayerController::AbilityInputTagReleased, &AHASPlayerController::AbilityInputTagHeld);
	}

}

void AHASPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}
