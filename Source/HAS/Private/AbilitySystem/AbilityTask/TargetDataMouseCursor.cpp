#include "AbilitySystem/AbilityTask/TargetDataMouseCursor.h"
#include "AbilitySystemComponent.h"

UTargetDataMouseCursor* UTargetDataMouseCursor::CreatemMouseCursorTargetData(UGameplayAbility* OwningAbility)
{
	UTargetDataMouseCursor* MyObject = NewAbilityTask<UTargetDataMouseCursor>(OwningAbility);
	return MyObject;
}

void UTargetDataMouseCursor::Activate()
{
	Super::Activate();

	const bool bIsLocal = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	
	// Local Player의 MouseCursorTargetData를 Server에 Send.
	if (bIsLocal)
	{
		SendMouseCursorTargetData();
	}
	else
	{
		// Server에 TargetData가 Set됐을 때 호출할 함수를 Binding.
		/* Generic callback for returning when target data is available
		 DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityTargetDataSetDelegate, const FGameplayAbilityTargetDataHandle&, FGameplayTag); */
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(Ability->GetCurrentAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UTargetDataMouseCursor::MouseCursorTargetDataCallback);

		// Server에 TargetData가 Set됐는지.
		const bool bIsSetTargetData = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(Ability->GetCurrentAbilitySpecHandle(), GetActivationPredictionKey());
		if (!bIsSetTargetData)
		{
			// Server에 TargetData가 Set될 때까지 Wait.
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataMouseCursor::SendMouseCursorTargetData()
{
	FScopedPredictionWindow ScopedPredictionWindo(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult MouseCursorHitResult;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, MouseCursorHitResult);

	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(MouseCursorHitResult);

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(TargetData);

	// Batching client->server RPCs
	// This is a WIP feature to batch up client->server communication. It is opt in and not complete. It only batches the below functions. Other Server RPCs are not safe to call during a batch window. Only opt in if you know what you are doing!
	// Client의 TargetData를 Server에 Set.
	AbilitySystemComponent->CallServerSetReplicatedTargetData(
		Ability->GetCurrentAbilitySpecHandle(),
		GetActivationPredictionKey(),
		TargetDataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	// Ability가 Activate 중이라면 true.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}

void UTargetDataMouseCursor::MouseCursorTargetDataCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ActivationTag)
{
	// Client에서 Replicat된 TargetData를 Clear.
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(Ability->GetCurrentAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}
