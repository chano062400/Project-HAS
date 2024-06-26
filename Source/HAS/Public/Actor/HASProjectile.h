#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "HASProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundBase;

UCLASS()
class HAS_API AHASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AHASProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool bFireWall;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Destroyed() override;

	virtual void OnHit();

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ImpactGameplayCueTag;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TrailGameplayCueTag;

	bool bHit = false;

	FTimerHandle FireWallHandle;

};
