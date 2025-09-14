#include "Inventory/InventoryComponent.h"

#include "Inventory/ItemInterface.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

EAddItemResult UInventoryComponent::AddItemToInventory(TScriptInterface<IItemInterface> Item)
{
	if (IsFull())
	{
		return EAddItemResult::InventoryFull;
	}

	for (FItemRepresentation& ItemSlot : Items)
	{
		if (ItemSlot.IsEmpty())
		{
			ItemSlot.Count = 1;
			ItemSlot.ItemDetails = Item->GetItemDetails();
			Item->OnAddedToInventory();
			return EAddItemResult::Success;
		}
	}

	return EAddItemResult::InventoryFull;
}
EAddItemResult UInventoryComponent::AddItemToInventorySlot(TScriptInterface<IItemInterface> Item, int32 SlotIndex)
{
	if (SlotIndex < 0 or SlotIndex >= MaxItemCount)
	{
		return EAddItemResult::InvalidSlot;
	}

	FItemRepresentation& Slot = Items[SlotIndex];

	bool bSameType = Slot.ItemDetails.ItemClassId == Item->GetItemDetails().ItemClassId;
	if (Slot.IsEmpty() or bSameType)
	{
		++Slot.Count;
		Slot.ItemDetails = Item->GetItemDetails();
		Item->OnAddedToInventory();
		return EAddItemResult::Success;
	}

	return EAddItemResult::ItemTypeMismatch;
}

int32 UInventoryComponent::GetItemCount() const
{
	return CurrentItemCount;
}

bool UInventoryComponent::IsFull() const
{
	return CurrentItemCount == MaxItemCount;
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
void UInventoryComponent::ResetSlot(TArray<FItemRepresentation>::SizeType Index)
{
	Items[0] = {};
}
