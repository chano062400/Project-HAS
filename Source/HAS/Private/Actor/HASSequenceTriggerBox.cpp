#include "Actor/HASSequenceTriggerBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "Character/HASEnemy.h"

AHASSequenceTriggerBox::AHASSequenceTriggerBox()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
}

void AHASSequenceTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AHASSequenceTriggerBox::BeginOverlap);

	UGameplayStatics::GetActorOfClass(GetWorld(), PlayEnemy)->SetActorHiddenInGame(true);
}

void AHASSequenceTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHASSequenceTriggerBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALevelSequenceActor* LevelSequenceActor;
	FMovieSceneSequencePlaybackSettings PlayBackSettings;
	PlayBackSettings.bDisableLookAtInput = true;
	PlayBackSettings.bDisableMovementInput = true;
	PlayBackSettings.bHideHud = true;
	PlayBackSettings.bHidePlayer = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlayBackSettings, LevelSequenceActor);
	LevelSequencePlayer->OnFinished.AddDynamic(this, &AHASSequenceTriggerBox::SequenceFinished);
	LevelSequencePlayer->Play();

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHASSequenceTriggerBox::SequenceFinished()
{
	UGameplayStatics::GetActorOfClass(GetWorld(), SequenceEnemy)->Destroy();
	
	if(AHASEnemy* Enemy = Cast<AHASEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), PlayEnemy)))
	{
		Enemy->SetActorHiddenInGame(false);
		Enemy->InitializeBehaviorTree();
	}

	Destroy();
}
