#include "HASAssetManager.h"
#include "HASGameplayTags.h"

void UHASAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FHASGameplayTags::Get().InitializeNativeGameplayTags();
}
