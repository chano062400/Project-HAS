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

	// Actor에게 커서가 있는지
	bTargeting = ThisActor ? true : false;
	bClicked = true;
	bAutoRun = false;

}

void AHASPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	UHASAbilitySystemComponent* ASC = Cast<UHASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	if(ASC == nullptr) return;
	
	// LMB(이동)가 아니라면 
	if (!InputTag.MatchesTagExact(FHASGameplayTags::Get().Input_LMB))
	{
		ASC->AbilityInputTagHeld(InputTag);
		return;
	}

	// Actor에게 LMB했다면
	if (bTargeting)
	{
		/* TODO 
		Actor와의 거리가 스킬 사거리 이상이라면 사거리까지 자동 이동 후 Activate */

		ASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		ElapsedTime += GetWorld()->GetDeltaSeconds();
		
		// LMB클릭한 지점을 목표로 설정.
		if(GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, MouseCursorHitResult))
		{
			CachedDestination = MouseCursorHitResult.ImpactPoint;
		}

		// 목표로 이동.
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
	
	// LMB(이동)가 아니라면 
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
			// 목적지까지의 Path를 찾음.
			if (UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				// 목적지까지의 Path Point를 Spline에 추가.
				for (const FVector& Point : Path->PathPoints)
				{
					Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), Point, 12.f, 12, FColor::Red, false, 5.f);
				}
				// 목적지를 Path의 마지막 지점으로 설정하고 자동이동.
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
		// Actor에 커서가 있음. 
		if (LastActor)
		{
			// 같은 엑터에 커서.
			if (ThisActor == LastActor)
			{
				ThisActor->HighlightActor();
			}
			// 다른 엑터에 커서.
			else 
			{
				ThisActor->HighlightActor();
				LastActor->UnHighlightActor();
			}
		}
		// 밖에 있다가 Actor에 커서를 갖다 댐.
		else
		{
			ThisActor->HighlightActor();
		}

	}
	else
	{
		CurrentMouseCursor = EMouseCursor::Default;
		// Actor에 커서를 대다가 밖으로 나옴. 
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
		// Spline Point에서 거리가 가장 가까운 Point를 반환..
		const FVector ClosestSplinePoint = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		
		// 방향 설정.
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(ClosestSplinePoint, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		// 자동이동 허용 반경 안이라면 자동이동 취소.
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
