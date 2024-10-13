#include "Actor/HASItem.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/HASInventoryWidget.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/HASCombatInterface.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/HASInventoryComponent.h"

AHASItem::AHASItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	NameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Name Widget"));
	NameWidget->SetupAttachment(Mesh);;
	NameWidget->SetVisibility(true);
	
}

void AHASItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AHASItem::OnSphereOverlap);

	if (UHASInventoryWidget* Widget = Cast<UHASInventoryWidget>(NameWidget->GetUserWidgetObject()))
	{
		Widget->ThisItemStruct = ItemStruct;
		Widget->ThisItem = this;
	}

	// DataTable 서버에만 존재.
	if (HasAuthority())
	{
		if (IsValid(ItemStruct.ItemHandle.DataTable))
		{
			FName Name = ItemStruct.ItemHandle.RowName;
			FItemInfo* ItemInfo = ItemStruct.ItemHandle.DataTable->FindRow<FItemInfo>(Name, "");
			Mesh->SetStaticMesh(ItemInfo->Mesh);
		}

		SetActorRotation(FRotator(45.f, 0.f, 0.f));
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(true);
		Mesh->AddImpulse(FVector(0.f, 0.f, SpawnImpulse));
	}

}

void AHASItem::OnRep_ItemStruct()
{
	if (IsValid(ItemStruct.ItemHandle.DataTable))
	{
		FName Name = ItemStruct.ItemHandle.RowName;
		FItemInfo* ItemInfo = ItemStruct.ItemHandle.DataTable->FindRow<FItemInfo>(Name, "");
		Mesh->SetStaticMesh(ItemInfo->Mesh);
	}
}

void AHASItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ItemStruct.ItemType != EItemType::EIT_Potion || OtherActor->ActorHasTag(FName("Enemy"))) return;

	FDataTableRowHandle ItemHandle = ItemStruct.ItemHandle;
	FItemInfo* Info = ItemHandle.DataTable->FindRow<FItemInfo>(ItemHandle.RowName, "");

	if (HasAuthority())
	{
		if (ItemStruct.ItemType == EItemType::EIT_Potion && ItemStruct.PotionType != EPotionType::EPT_Elixir)
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				if (OtherActor->Implements<UHASCombatInterface>())
				{
					if (IHASCombatInterface* Interface = Cast<IHASCombatInterface>(OtherActor))
					{
						for (const auto& Effect : Info->UseEffects)
						{
							int32 PlayerLevel = Interface->Execute_GetLevel(OtherActor);
							FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
							FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Effect, PlayerLevel, EffectContextHandle);
							ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

							Destroy();
						}
					}
				}
			}
		}
	}
}

void AHASItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHASItem, ItemStruct);
}
