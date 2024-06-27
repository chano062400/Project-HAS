#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HASMagicCircle.generated.h"

class UDecalComponent;

UCLASS()
class HAS_API AHASMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AHASMagicCircle();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleComponent;

protected:
	virtual void BeginPlay() override;


};
