#include "Inventory/HASInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UHASInventoryComponent::UHASInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Inventory.Reserve(30);
}


void UHASInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHASInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHASInventoryComponent, Inventory, COND_OwnerOnly);
}

void UHASInventoryComponent::OnRep_Inventory()
{
}

