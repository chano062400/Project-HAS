#include "Actor/HASAbilityAreaIndicator.h"
#include "Components/TimelineComponent.h"
#include "Components/DecalComponent.h"
#include "Interfaces/HASCombatInterface.h"
#include "Net/UnrealNetwork.h"

AHASAbilityAreaIndicator::AHASAbilityAreaIndicator()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BorderDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Border Decal"));
	BorderDecal->SetupAttachment(Root);

	AreaDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Area Decal"));
	AreaDecal->SetupAttachment(Root);

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

void AHASAbilityAreaIndicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHASAbilityAreaIndicator, IndicatorDecalSize);
	DOREPLIFETIME(AHASAbilityAreaIndicator, TimelineCurve);
	DOREPLIFETIME(AHASAbilityAreaIndicator, IndicatorShape);
	DOREPLIFETIME(AHASAbilityAreaIndicator, IndicatorLength);
}

void AHASAbilityAreaIndicator::SetIndicatorMaterial()
{
	switch (IndicatorShape)
	{
	case EIndicatorShape::EIS_Circle:
		IndicatorMaterial = CircleMaterial;
		break;
	case EIndicatorShape::EIS_Radial:
		IndicatorMaterial = RadialMaterial;
		break;
	case EIndicatorShape::EIS_Box:
		IndicatorMaterial = BoxMaterial;
		break;
	}
}

void AHASAbilityAreaIndicator::OnRep_IndicatorDecalSize()
{
	if (IsValid(BorderDecal))
	{
		BorderDecal->DecalSize = IndicatorDecalSize;
	}

	if (IsValid(AreaDecal))
	{
		AreaDecal->DecalSize = IndicatorDecalSize;
	}
}

void AHASAbilityAreaIndicator::OnRep_IndicatorShape()
{
	SetIndicatorMaterial();

	if (IsValid(BorderDecal))
	{
		BorderDecal->SetMaterial(0, IndicatorMaterial);
	}

	if (IsValid(AreaDecal))
	{
		AreaDecal->SetMaterial(0, IndicatorMaterial);
	}
}

void AHASAbilityAreaIndicator::OnRep_TimelineCurve()
{
	if (IsValid(Timeline) && IsValid(TimelineCurve))
	{
		Timeline->AddInterpFloat(TimelineCurve, TimelineUpdate);
	}
}

void AHASAbilityAreaIndicator::OnRep_IndicatorLength()
{
	if (IsValid(Timeline))
	{
		Timeline->SetTimelineLength(IndicatorLength);
	}
}

void AHASAbilityAreaIndicator::InitializeMaterial()
{
	if (IndicatorMaterial)
	{
		DynamicBorderMaterialInstance = UMaterialInstanceDynamic::Create(IndicatorMaterial, this);
		DynamicAreaMaterialInstance = UMaterialInstanceDynamic::Create(IndicatorMaterial, this);

		BorderDecal->SetMaterial(0, DynamicBorderMaterialInstance);
		AreaDecal->SetMaterial(0, DynamicAreaMaterialInstance);

		DynamicBorderMaterialInstance->SetScalarParameterValue(FName("CircleValue"), 1.f);
		DynamicAreaMaterialInstance->SetScalarParameterValue(FName("CircleValue"), 0.f);
	}
}

void AHASAbilityAreaIndicator::InitializeTimeline()
{
	if (TimelineCurve)
	{
		Timeline->SetLooping(false);
		TimelineUpdate.BindUFunction(this, FName("UpdateArea"));
		Timeline->AddInterpFloat(TimelineCurve, TimelineUpdate);
		TimelineFinished.BindUFunction(this, FName("Finish"));
		Timeline->SetTimelineFinishedFunc(TimelineFinished);
		Timeline->SetTimelineLength(IndicatorLength);
	}
}

void AHASAbilityAreaIndicator::MultiSpawnIndicator_Implementation()
{
	SetIndicatorMaterial();

	if (IndicatorShape != EIndicatorShape::EIS_Box)
	{
		BorderDecal->DecalSize = IndicatorDecalSize;

		AreaDecal->DecalSize = IndicatorDecalSize;

		SetSpawnZ();

		InitializeMaterial();

		InitializeTimeline();

		Timeline->PlayFromStart();
	}
	else
	{
		BorderDecal->SetMaterial(0, IndicatorMaterial);
		AreaDecal->DestroyComponent();

		if (GetOwner()->Implements<UHASCombatInterface>())
		{
			if (IHASCombatInterface* Interface = Cast<IHASCombatInterface>(GetOwner()))
			{
				AActor* TargetActor = Interface->Execute_GetCombatTarget(GetOwner());
				if (IsValid(TargetActor))
				{
					float Distance = GetOwner()->GetDistanceTo(TargetActor);
					FVector ToEnemy = TargetActor->GetActorLocation() - GetOwner()->GetActorLocation();

					SetActorLocation(GetActorLocation() + ToEnemy);
					SetActorRotation(ToEnemy.ToOrientationQuat());
					SetSpawnZ();

					// Replicate 시키기 위해 
					IndicatorDecalSize = FVector(IndicatorDecalSize.X, IndicatorDecalSize.Y, Distance + 500.f);
					BorderDecal->DecalSize = IndicatorDecalSize;
				}
			}
		}

		FTimerHandle IndicatorTimer;
		GetWorld()->GetTimerManager().SetTimer(IndicatorTimer, [this]() { Destroy(); }, IndicatorLength, false);
	}
}

void AHASAbilityAreaIndicator::SetSpawnZ()
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, -100.f),
		FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 100.f),
		ECollisionChannel::ECC_Visibility
	);
	
	if (HitResult.bBlockingHit)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, HitResult.ImpactPoint.Z));
	}
}

void AHASAbilityAreaIndicator::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		MultiSpawnIndicator();
	}
	
}

void AHASAbilityAreaIndicator::UpdateArea(float Value)
{
	if (DynamicAreaMaterialInstance)
	{
		DynamicAreaMaterialInstance->SetScalarParameterValue(FName("AreaValue"), Value);
		UE_LOG(LogTemp, Error, TEXT("AreaValue : %f"),Value);
	}
}

void AHASAbilityAreaIndicator::Finish()
{
	Destroy();
}

