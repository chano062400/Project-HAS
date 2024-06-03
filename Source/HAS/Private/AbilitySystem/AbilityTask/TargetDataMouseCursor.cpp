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
	
	// Local Player�� MouseCursorTargetData�� Server�� Send.
	if (bIsLocal)
	{
		SendMouseCursorTargetData();
	}
	else
	{
		// Server�� TargetData�� Set���� �� ȣ���� �Լ��� Binding.
		/* Generic callback for returning when target data is available
		 DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityTargetDataSetDelegate, const FGameplayAbilityTargetDataHandle&, FGameplayTag); */
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(Ability->GetCurrentAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UTargetDataMouseCursor::MouseCursorTargetDataCallback);

		// Server�� TargetData�� Set�ƴ���.
		const bool bIsSetTargetData = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(Ability->GetCurrentAbilitySpecHandle(), GetActivationPredictionKey());
		if (!bIsSetTargetData)
		{
			// Server�� TargetData�� Set�� ������ Wait.
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
	// Client�� TargetData�� Server�� Set.
	AbilitySystemComponent->CallServerSetReplicatedTargetData(
		Ability->GetCurrentAbilitySpecHandle(),
		GetActivationPredictionKey(),
		TargetDataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	// Ability�� Activate ���̶�� true.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}

void UTargetDataMouseCursor::MouseCursorTargetDataCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ActivationTag)
{
	// Client���� Replicat�� TargetData�� Clear.
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(Ability->GetCurrentAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}
