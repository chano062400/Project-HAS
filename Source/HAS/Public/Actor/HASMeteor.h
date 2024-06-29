#pragma once

#include "CoreMinimal.h"
#include "Actor/HASProjectile.h"
#include "HASMeteor.generated.h"

/**
 * 
 */
UCLASS()
class HAS_API AHASMeteor : public AHASProjectile
{
	GENERATED_BODY()

public:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
