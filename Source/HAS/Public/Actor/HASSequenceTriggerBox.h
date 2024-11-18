#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HASSequenceTriggerBox.generated.h"

class UBoxComponent;
class ULevelSequence;
class ULevelSequencePlayer;

UCLASS()
class HAS_API AHASSequenceTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AHASSequenceTriggerBox();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerBeginOverlap();

	UFUNCTION(NetMulticast , Reliable)
	void MultiPlaySequence();

	UFUNCTION()
	void SequenceFinished();

	UPROPERTY(EditAnywhere)
	TObjectPtr<ULevelSequence> LevelSequence;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SequenceEnemy;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayEnemy;

};
