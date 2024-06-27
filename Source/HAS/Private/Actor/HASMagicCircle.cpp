#include "Actor/HASMagicCircle.h"
#include "Components/DecalComponent.h"

AHASMagicCircle::AHASMagicCircle()
{
	PrimaryActorTick.bCanEverTick = false;

	MagicCircleComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Magic Circle"));
	MagicCircleComponent->SetupAttachment(GetRootComponent());
}

void AHASMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}
