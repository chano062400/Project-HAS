#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "HASAbilityAreaIndicator.generated.h"

UENUM(BlueprintType)
enum class EIndicatorShape : uint8
{
	EIS_None UMETA(DisplayName = "None"),
	EIS_Circle UMETA(DisplayName = "Circle"),
	EIS_Radial UMETA(DisplayName = "Radial"),
	EIS_Box UMETA(DisplayName = "Box"),
};

UCLASS()
class HAS_API AHASAbilityAreaIndicator : public AActor
{
	GENERATED_BODY()
	
public:	

	AHASAbilityAreaIndicator();

protected:

	virtual void BeginPlay() override;

	void TimelineStart();

	void InitializeMaterial();

	void SetIndicatorMaterial();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FVector IndicatorDecalSize = FVector(100.f, 100.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	EIndicatorShape IndicatorShape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UCurveFloat> TimelineCurve;

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDecalComponent> BorderDecal;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDecalComponent> AreaDecal;

	UFUNCTION()
	void UpdateArea(float Value);

	UFUNCTION()
	void Finish();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> CircleMaterial;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> RadialMaterial;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> BoxMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstance> IndicatorMaterial;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> Timeline;

	FOnTimelineFloat TimelineUpdate;
	
	FOnTimelineEvent TimelineFinished;

	UPROPERTY(EditDefaultsOnly)
	float TimelineLength = 1.f;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicBorderMaterialInstance;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicAreaMaterialInstance;

};
