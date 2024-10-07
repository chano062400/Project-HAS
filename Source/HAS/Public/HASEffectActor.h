#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HASAbilityTypes.h"
#include "Engine/DataTable.h"
#include "HASEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;
struct FScalableFloat;
class UNiagaraSystem;
class UWidgetComponent;


UCLASS()
class HAS_API AHASEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AHASEffectActor();

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	void SetDamageEffectTargetASC(UAbilitySystemComponent* AbilitySystemComponent);

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float LifeSpan;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FHASDamageEffectParams DamageEffectParams;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> LoopingSound;

};
