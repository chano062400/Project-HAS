#include "HASAssetManager.h"
#include "HASGameplayTags.h"
#include "AbilitySystemGlobals.h"

void UHASAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FHASGameplayTags::Get().InitializeNativeGameplayTags();

	UAbilitySystemGlobals::Get().InitTargetDataScriptStructCache();
}