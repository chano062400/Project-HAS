#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "HASAbilityTypes.h"
#include "HASProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSphereOverlapSignature, const FVector&, OverlapLocation);

UCLASS()
class HAS_API AHASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AHASProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	//UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	//FGameplayEffectSpecHandle DamageEffectSpecHandle;
	//
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FHASDamageEffectParams DamageEffectParams;

	UPROPERTY(BlueprintAssignable)
	FOnSphereOverlapSignature OnSphereOverlapDelegate;

	UFUNCTION(BlueprintCallable)
	void SetDamageEffectTargetASC(UAbilitySystemComponent* AbilitySystemComponent);

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Destroyed() override;

	virtual void OnHit();
	
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

	bool bHit = false;

	UPROPERTY(EditDefaultsOnly)
	bool IsOverlapDestroy = true;
};


