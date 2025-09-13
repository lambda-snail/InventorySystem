#include "Inventory/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

EAddItemResult UInventoryComponent::AddItemToInventory(TScriptInterface<IItemInterface> Item)
{
	return EAddItemResult::Success;
}

int32 UInventoryComponent::GetItemCount() const
{
	return Items.Num();
}

bool UInventoryComponent::IsFull() const
{
	return Items.Num() == MaxItemCount;
}

int32 UInventoryComponent::GetMaxItemCount() const
{
	return MaxItemCount;
}

void UInventoryComponent::SetMaxItemCount(int32 NewCount)
{
}
void UInventoryComponent::ForeachItem(TFunction<void(int32 ItemClassID, int32 ItemInstanceID)> Callback) const
{
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
void UInventoryComponent::EndPlay(EEndPlayReason::Type const EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
