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
#include "UI/Widget/DamageTextComponent.h"
#include "Actor/HASMagicCircle.h"
#include "Components/DecalComponent.h"
#include "Character/HASCharacter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Actor/HASItem.h"
#include "Engine/TextureRenderTarget2D.h"
#include "NiagaraFunctionLibrary.h"

AHASPlayerController::AHASPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

}

void AHASPlayerController::ClientShowEnemyInfo_Implementation(FEnemyInfo Info, int32 PlayerLevel)
{
	EnemyInfo.Broadcast(Info, PlayerLevel);
}

AHASMagicCircle* AHASPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (IsValid(MagicCircleDecalComponent))
	{
		MagicCircle = GetWorld()->SpawnActor<AHASMagicCircle>(MagicCircleDecalComponent, MouseCursorHitResult.ImpactPoint, FRotator::ZeroRotator);
		if (IsValid(MagicCircle))
		{
			MagicCircle->MagicCircleComponent->SetDecalMaterial(DecalMaterial);
		}
		return MagicCircle;
	}
	return nullptr;
}

void AHASPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AHASPlayerController::UpdateMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(MouseCursorHitResult.ImpactPoint);
	}
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

		if (bOpenedSpellMenu)
		{
			if (UHASUserWidget* Widget = HASHUD->SpellMenuWidget)
			{
				bOpenedSpellMenu = false;
				Widget->RemoveFromParent();
			}
		}
		if (bOpenedInventory)
		{
			if (UHASUserWidget* Widget = HASHUD->InventoryWidget)
			{
				bOpenedInventory = false;
				Widget->RemoveFromParent();
			}
		}
	}
}

void AHASPlayerController::OpenSpellMenu()
{
	if (AHASHUD* HASHUD = GetHUD<AHASHUD>())
	{
		check(HASHUD->SpellMenuWidget);

		if (!bOpenedSpellMenu)
		{
			bOpenedSpellMenu = true;

			if (UHASUserWidget* Widget = HASHUD->SpellMenuWidget)
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
			bOpenedSpellMenu = false;
			HASHUD->SpellMenuWidget->RemoveFromParent();
		}

		if (bOpenedAttributeMenu)
		{
			if (UHASUserWidget* Widget = HASHUD->AttributeMenuWidget)
			{
				bOpenedAttributeMenu = false;
				Widget->RemoveFromParent();
			}
		}
		if (bOpenedInventory)
		{
			if (UHASUserWidget* Widget = HASHUD->InventoryWidget)
			{
				bOpenedInventory = false;
				Widget->RemoveFromParent();
			}
		}
	}
}

void AHASPlayerController::OpenInventory()
{
	if (AHASHUD* HASHUD = GetHUD<AHASHUD>())
	{
		check(HASHUD->InventoryWidget);

		if (!bOpenedInventory)
		{
			bOpenedInventory = true;

			if (UHASUserWidget* Widget = HASHUD->InventoryWidget)
			{
				Widget->AddToViewport();
			}
		}
		else
		{
			bOpenedInventory= false;
			HASHUD->InventoryWidget->RemoveFromParent();
		}

		if (bOpenedAttributeMenu)
		{
			if (UHASUserWidget* Widget = HASHUD->AttributeMenuWidget)
			{
				bOpenedAttributeMenu = false;
				Widget->RemoveFromParent();
			}
		}
		
		if (bOpenedSpellMenu)
		{
			if (UHASUserWidget* Widget = HASHUD->SpellMenuWidget)
			{
				bOpenedSpellMenu = false;
				Widget->RemoveFromParent();
			}
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

	// Actor���� LMB or Shift�� ������ �ִٸ�
	if (bTargeting || bShiftPressed)
	{
		/* TODO 
		Actor���� �Ÿ��� ��ų ��Ÿ� �̻��̶�� ��Ÿ����� �ڵ� �̵� �� Activate */

		ASC->AbilityInputTagHeld(InputTag);
		
		// �̵��ϴ� Ability�� ����ϸ� ���ߵ���.
		bAutoRun = false;
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
				}
				// �������� Path�� ������ �������� �����ϰ� �ڵ��̵�.
				if (Path->PathPoints.Num() > 0)
				{
					CachedDestination = Path->PathPoints.Last();
					bAutoRun = true;
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickEffect, CachedDestination);
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
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
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
	if (!bAutoRun)
	{
		StopMovement();
		return;
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		//  �Ÿ��� ���� ����� SplinePoint�� ��ȯ..
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

void AHASPlayerController::ShiftPressed()
{
	if (!bShiftPressed)
	{
		bShiftPressed = true;
	}
}

void AHASPlayerController::ShiftReleased()
{
	if (bShiftPressed)
	{
		bShiftPressed = false;
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
		HASEnhancedInputComponent->BindAction(SpellMenuAction, ETriggerEvent::Triggered, this, &AHASPlayerController::OpenSpellMenu);
		HASEnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AHASPlayerController::OpenInventory);
		HASEnhancedInputComponent->BindAction(ShiftPressedAction, ETriggerEvent::Started, this, &AHASPlayerController::ShiftPressed);
		HASEnhancedInputComponent->BindAction(ShiftReleasedAction, ETriggerEvent::Completed, this, &AHASPlayerController::ShiftReleased);
		HASEnhancedInputComponent->BindAbilityAction(InputInfo, this, &AHASPlayerController::AbilityInputTagPressed, &AHASPlayerController::AbilityInputTagReleased, &AHASPlayerController::AbilityInputTagHeld);
	}

}

void AHASPlayerController::ClientShowFloatingDamageText_Implementation(float Damage, AActor* TargetActor, bool bIsCritical)
{
	if (IsValid(TargetActor) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetActor, DamageTextComponentClass);
		//NewObject�� ������ Component�� �ݵ�� ���(Register)������ ���ľ� ��
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(Damage, bIsCritical);
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [DamageText]() {DamageText->DestroyComponent(); }, false, 1.f);
	}
}

void AHASPlayerController::ClientShowFloatingImmnueText_Implementation(AActor* TargetActor)
{
	if (IsValid(TargetActor) && DamageTextComponentClass)
	{
		UWidgetComponent* ImmuneText = NewObject<UWidgetComponent>(TargetActor, ImmuneTextComponentClass);
	
		ImmuneText->RegisterComponent();
		ImmuneText->AttachToComponent(TargetActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		ImmuneText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [ImmuneText]() {ImmuneText->DestroyComponent(); }, false, 1.f);
	}
}

void AHASPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();

	AutoRun();

	UpdateMagicCircle();
}
