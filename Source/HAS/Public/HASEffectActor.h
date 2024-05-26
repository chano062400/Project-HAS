#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HASEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;
struct FScalableFloat;

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

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
