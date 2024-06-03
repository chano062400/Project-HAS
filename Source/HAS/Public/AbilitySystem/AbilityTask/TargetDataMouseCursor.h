#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataMouseCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseCursorTargetDataSignature, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class HAS_API UTargetDataMouseCursor : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "MouseCursorTargetData", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataMouseCursor* CreatemMouseCursorTargetData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseCursorTargetDataSignature ValidData;

private:

	virtual void Activate() override;

	void SendMouseCursorTargetData();

	void MouseCursorTargetDataCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ActivationTag);
};
