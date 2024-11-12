#include "Actor/HASAbilityAreaIndicator.h"
#include "Components/TimelineComponent.h"
#include "Components/DecalComponent.h"
#include "Interfaces/HASCombatInterface.h"

AHASAbilityAreaIndicator::AHASAbilityAreaIndicator()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BorderDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Border Decal"));
	BorderDecal->SetupAttachment(Root);

	AreaDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Area Decal"));
	AreaDecal->SetupAttachment(Root);

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
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

void AHASAbilityAreaIndicator::BeginPlay()
{
	Super::BeginPlay();
	
	SetIndicatorMaterial();

	if (IndicatorShape != EIndicatorShape::EIS_Box)
	{
		BorderDecal->DecalSize = IndicatorDecalSize;

		AreaDecal->DecalSize = IndicatorDecalSize;

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
				float Distance = GetOwner()->GetDistanceTo(TargetActor);
				FVector ToEnemy = TargetActor->GetActorLocation() - GetOwner()->GetActorLocation();

				SetActorLocation(GetActorLocation() + ToEnemy);
				SetActorRotation(ToEnemy.ToOrientationQuat());
				BorderDecal->DecalSize = FVector(1.f, IndicatorDecalSize.Y, Distance);
			}
		}
		FTimerHandle IndicatorTimer;
		GetWorld()->GetTimerManager().SetTimer(IndicatorTimer, [this]() { Destroy(); }, IndicatorLength, false);
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

