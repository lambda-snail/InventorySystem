#include "Inventory/ContainerBase.h"

#include "Inventory/InventoryComponent.h"

AContainerBase::AContainerBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true);
}

bool AContainerBase::CanInteract_Implementation() const
{
	return true;
}

void AContainerBase::Interact_Implementation(AActor* Interactor) {}

void AContainerBase::BeginPlay()
{
	Super::BeginPlay();
}
