#include "Actor/HASMeteor.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/HASAbilitySystemBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HASGameplayTags.h"
#include "Player/HASPlayerController.h"

void AHASMeteor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bHit) OnHit();

	if (HasAuthority())
	{
		OnSphereOverlapDelegate.Broadcast(OtherActor);
	}
	else bHit = true;
}
