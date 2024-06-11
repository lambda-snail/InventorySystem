#include "Inventory/Pickup.h"

#include "Inventory/InventoryLog.h"
#include "InertItem.h"
#include "Components/SphereComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemBase.h"
#include "Inventory/InventoryActor.h"
#include "Logging/StructuredLog.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	Trigger = CreateDefaultSubobject<USphereComponent>("Trigger");
	Trigger->SetupAttachment(Root);
	Trigger->SetCanEverAffectNavigation(false); 

	Trigger->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Trigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(Root);
}

void APickup::OnPickUp(AActor* Agent)
{
	if(Agent->Implements<UInventoryActor>())
	{
		OnPickup(TScriptInterface<IInventoryActor>(Agent));
	}
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	if(not Item)
	{
		UClass const* ItemBaseClass = ItemClass.Get();
		UObject* CreatedItem = NewObject<UInertItem>(this, ItemBaseClass);
		Item = Cast<UInertItem>(CreatedItem);
	}

	StaticMeshComponent->SetStaticMesh(Item->GetMesh());

	Super::BeginPlay();
}

void APickup::OnPickup(TScriptInterface<IInventoryActor> const& PickupCharacter)
{
	UInventoryComponent* Inventory = PickupCharacter->GetInventoryComponent();
	
	EItemAddResult Result = Inventory->TryAddItem(Item);
	if(Result == EItemAddResult::Success)
	{
		// TODO: Add to item pool?
		Item->OnPickup(Cast<AActor>(PickupCharacter.GetObject()));
		Item = nullptr; // Prevent destruction of item, TODO: Add to actor pool
		this->Destroy();
	}
	else
	{
		// TODO: Some kind of feedback when adding fails?
	}
}

bool APickup::CanInteract_Implementation() const
{
	return true;
}

void APickup::Interact_Implementation(AActor* Interactor)
{
	if(Interactor->Implements<UInventoryActor>())
	{
		OnPickup(Interactor);
	}
	else
	{
		UE_LOGFMT(LambdaSnailInventory, Warning, "Pickup ({This}) received an interaction from {Object}, but this actoer is not an IItemUser", GetName(), Interactor->GetName());
	}
}

bool APickup::IsLocalInteraction_Implementation() const
{
	return false;
}
