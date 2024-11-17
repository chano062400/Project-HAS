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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

	void InitializeTimeline();

	void InitializeMaterial();

	void SetIndicatorMaterial();

	UPROPERTY(ReplicatedUsing = OnRep_IndicatorDecalSize, EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FVector IndicatorDecalSize = FVector(100.f, 100.f, 100.f);

	UFUNCTION()
	void OnRep_IndicatorDecalSize();

	UPROPERTY(ReplicatedUsing = OnRep_IndicatorShape, EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	EIndicatorShape IndicatorShape;

	UFUNCTION()
	void OnRep_IndicatorShape();

	UPROPERTY(ReplicatedUsing = OnRep_TimelineCurve, EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UCurveFloat> TimelineCurve;

	UFUNCTION()
	void OnRep_TimelineCurve();

	UPROPERTY(ReplicatedUsing = OnRep_IndicatorLength, EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float IndicatorLength = 1.f;

	UFUNCTION()
	void OnRep_IndicatorLength();

private:

	UFUNCTION(NetMulticast, Reliable)
	void MultiSpawnIndicator();

	void SetSpawnZ();

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

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicBorderMaterialInstance;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicAreaMaterialInstance;

};
