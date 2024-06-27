#include "Actor/HASProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HAS/HAS.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayCueManager.h"
#include "Interfaces/HASCombatInterface.h"

AHASProjectile::AHASProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	// Projectile Channel�� ����.
	Sphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Sphere->SetGenerateOverlapEvents(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->ProjectileGravityScale = 0.f; // �߷� ���� X
}

void AHASProjectile::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AHASProjectile::OnSphereOverlap);

	SetReplicateMovement(true);

	SetLifeSpan(LifeSpan);

	if (LoopingSound) LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());

}

void AHASProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(UHASAbilitySystemBlueprintLibrary::IsFriend(OtherActor, GetOwner())) return;

	if(!bHit) OnHit();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if (DamageEffectSpecHandle.IsValid())
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

				Destroy();
			}
		}
	}
	// Ŭ���̾�Ʈ���� Overlap�� ���� �߻��� ���.
	else bHit = true;
	

}

void AHASProjectile::Destroyed()
{
	if (LoopingSoundComponent && LoopingSoundComponent->IsPlaying())
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	// Ŭ���̾�Ʈ���� Destroy�� ���� �߻��� ���.
	if (!bHit && !HasAuthority()) OnHit();

	Super::Destroyed();
}

void AHASProjectile::OnHit()
{
	FGameplayCueParameters CueParams;
	CueParams.Location = GetActorLocation();
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(this, ImpactGameplayCueTag, CueParams);

	if (LoopingSoundComponent && LoopingSoundComponent->IsPlaying())
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}

