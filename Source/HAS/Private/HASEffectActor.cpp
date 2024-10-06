#include "HASEffectActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

AHASEffectActor::AHASEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);

	NameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Name Widget"));
	NameWidget->SetupAttachment(Mesh);
	NameWidget->SetVisibility(true);
}

void AHASEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AHASEffectActor::OnSphereOverlap);

	if (!bIsPotion)
	{
		SetLifeSpan(LifeSpan);

		if (LoopingSound) LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	}
	else
	{
		SetActorRotation(FRotator(45.f, 0.f, 0.f));
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(true);
		Mesh->AddImpulse(FVector(0.f, 0.f, SpawnImpulse));
	}
}

void AHASEffectActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsPotion && OtherActor->ActorHasTag(FName("Player"))) return;
	if (bIsPotion && OtherActor->ActorHasTag(FName("Enemy"))) return;

	if (HasAuthority())
	{
		if (!bIsPotion)
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				DamageEffectParams.TargetASC = TargetASC;
				UHASAbilitySystemBlueprintLibrary::ApplyDamageEffectParams(DamageEffectParams);
			}
		}
		else
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
				FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(EffectClass, 1.f, EffectContext);
				ASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), ASC);
				Destroy();
			}
		}
	}
}

void AHASEffectActor::Destroyed()
{
	Super::Destroyed();

	if (LoopingSoundComponent && LoopingSoundComponent->IsPlaying())
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();

	}
}

void AHASEffectActor::SetDamageEffectTargetASC(UAbilitySystemComponent* AbilitySystemComponent)
{
	DamageEffectParams.TargetASC = AbilitySystemComponent;
}

