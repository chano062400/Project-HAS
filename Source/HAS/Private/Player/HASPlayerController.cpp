#include "Player/HASPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/HUD/HASHUD.h"
#include "UI/Widget/HASUserWidget.h"
#include "Input/HASEnhancedInputComponent.h"
#include "AbilitySystem/HASAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/HASCombatInterface.h"
#include "Components/SplineComponent.h"
#include "HASGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

AHASPlayerController::AHASPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

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
	UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	if (ASC == nullptr) return;
	
	ASC->AbilityInputTagPressed(InputTag);

	// Actor���� Ŀ���� �ִ���
	bTargeting = ThisActor ? true : false;
	bClicked = true;
	bAutoRun = false;

}

void AHASPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	if(ASC == nullptr) return;
	
	// LMB(�̵�)�� �ƴ϶�� 
	if (!InputTag.MatchesTagExact(FHASGameplayTags::Get().Input_LMB))
	{
		ASC->AbilityInputTagHeld(InputTag);
		return;
	}

	// Actor���� LMB�ߴٸ�
	if (bTargeting)
	{
		/* TODO 
		Actor���� �Ÿ��� ��ų ��Ÿ� �̻��̶�� ��Ÿ����� �ڵ� �̵� �� Activate */

		ASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		ElapsedTime += GetWorld()->GetDeltaSeconds();
		
		// LMBŬ���� ������ ��ǥ�� ����.
		if(GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, MouseCursorHitResult))
		{
			CachedDestination = MouseCursorHitResult.ImpactPoint;
		}

		// ��ǥ�� �̵�.
		if (APawn* ControlledPawn = GetPawn())
		{
			FVector ToDestination = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(ToDestination, 1.f);
		}
	}
	
}

void AHASPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	if (ASC == nullptr) return;
	
	// LMB(�̵�)�� �ƴ϶�� 
	if (!InputTag.MatchesTagExact(FHASGameplayTags::Get().Input_LMB))
	{
		ASC->AbilityInputTagReleased(InputTag);
		return;
	}

	ASC->AbilityInputTagReleased(InputTag);	
	
	bClicked = false;

	if(!bTargeting)
	{
		APawn* ControlledPawn = GetPawn();
	
		if (ControlledPawn && ElapsedTime < ShortPressThreshold)
		{
			// ������������ Path�� ã��.
			if (UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				// ������������ Path Point�� Spline�� �߰�.
				for (const FVector& Point : Path->PathPoints)
				{
					Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), Point, 12.f, 12, FColor::Red, false, 5.f);
				}
				// �������� Path�� ������ �������� �����ϰ� �ڵ��̵�.
				if (Path->PathPoints.Num() > 0)
				{
					CachedDestination = Path->PathPoints.Last();
					bAutoRun = true;
				}
			}
		}
		ElapsedTime = 0.f;
		bTargeting = false;
	}
}

void AHASPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, MouseCursorHitResult);
	if (!MouseCursorHitResult.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IHASCombatInterface>(MouseCursorHitResult.GetActor());

	if(ThisActor)
	{
		CurrentMouseCursor = bClicked ? EMouseCursor::GrabHand : EMouseCursor::Hand;
		// Actor�� Ŀ���� ����. 
		if (LastActor)
		{
			// ���� ���Ϳ� Ŀ��.
			if (ThisActor == LastActor)
			{
				ThisActor->HighlightActor();
			}
			// �ٸ� ���Ϳ� Ŀ��.
			else 
			{
				ThisActor->HighlightActor();
				LastActor->UnHighlightActor();
			}
		}
		// �ۿ� �ִٰ� Actor�� Ŀ���� ���� ��.
		else
		{
			ThisActor->HighlightActor();
		}

	}
	else
	{
		CurrentMouseCursor = EMouseCursor::Default;
		// Actor�� Ŀ���� ��ٰ� ������ ����. 
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
	}
}

void AHASPlayerController::AutoRun()
{
	if (!bAutoRun) return;

	if (APawn* ControlledPawn = GetPawn())
	{
		// Spline Point���� �Ÿ��� ���� ����� Point�� ��ȯ..
		const FVector ClosestSplinePoint = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		
		// ���� ����.
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(ClosestSplinePoint, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		// �ڵ��̵� ��� �ݰ� ���̶�� �ڵ��̵� ���.
		const float DistanceToDestination = (ClosestSplinePoint - CachedDestination).Length();
  		if (DistanceToDestination <= AutoRunAcceptance)
		{
			bAutoRun = false;
		}
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
	
	CursorTrace();

	AutoRun();
}
