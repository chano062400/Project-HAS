#include "Actor/HASItem.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/HASUserWidget.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/HASCombatInterface.h"

AHASItem::AHASItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

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
	NameWidget->SetupAttachment(Mesh);
	NameWidget->SetVisibility(true);
}

void AHASItem::ServerAddToInventory_Implementation(const FItemStruct& InItemStruct)
{
	Destroy();
}

void AHASItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AHASItem::OnSphereOverlap);

	FName Name = ItemStruct.ItemHandle.RowName;
	FItemInfo* ItemInfo = ItemStruct.ItemHandle.DataTable->FindRow<FItemInfo>(Name, "");
	Mesh->SetStaticMesh(ItemInfo->Mesh);

	if (UHASUserWidget* Widget = Cast<UHASUserWidget>(NameWidget->GetUserWidgetObject()))
	{
		Widget->ItemStruct = ItemStruct;
		Widget->ThisItem = this;
	}

	SetActorRotation(FRotator(45.f, 0.f, 0.f));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->AddImpulse(FVector(0.f, 0.f, SpawnImpulse));
	
}

void AHASItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ItemStruct.ItemType != EItemType::EIT_Potion || OtherActor->ActorHasTag(FName("Enemy"))) return;

	if (HasAuthority())
	{
		if (ItemStruct.ItemType == EItemType::EIT_Potion)
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				if (OtherActor->Implements<UHASCombatInterface>())
				{
					if (IHASCombatInterface* Interface = Cast<IHASCombatInterface>(OtherActor))
					{
						int32 PlayerLevel = Interface->Execute_GetLevel(OtherActor);
						FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
						FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, PlayerLevel, EffectContextHandle);
						ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

						Destroy();
					}
				}
			}
		}
	}
}
