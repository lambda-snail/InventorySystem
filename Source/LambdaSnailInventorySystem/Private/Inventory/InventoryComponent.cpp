#include "Inventory/InventoryComponent.h"

#if WITH_EDITOR
	#include "Inventory/ItemDataRow.h"
#endif

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

	for (int32 SlotIndex = 0; SlotIndex < MaxItemCount; ++SlotIndex)
	{
		FItemRepresentation& Slot = Items[SlotIndex];

		if (Slot.IsEmpty())
		{
			Slot.Count = 1;
			Slot.ItemDetails = IItemInterface::Execute_GetItemDetails(Item.GetObject());
			IItemInterface::Execute_OnAddedToInventory(Item.GetObject());
			OnItemAdded.Broadcast(Slot.ItemDetails.ItemClassId, Slot.ItemDetails.ItemInstanceId, SlotIndex);
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

		Slot.ItemDetails = IItemInterface::Execute_GetItemDetails(Item.GetObject());
		IItemInterface::Execute_OnAddedToInventory(Item.GetObject());
		OnItemAdded.Broadcast(Slot.ItemDetails.ItemClassId, Slot.ItemDetails.ItemInstanceId, SlotIndex);
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

void UInventoryComponent::ForeachSlot(TFunction<void(FName, int32, int32, int32)> const& Callback) const
{
	for (int32 Index = 0; Index < MaxItemCount; ++Index)
	{
		FItemRepresentation const& Slot = Items[Index];

		Callback(
			Slot.IsEmpty() ? FName() : Slot.ItemDetails.ItemClassId,
			Slot.IsEmpty() ? 0 : Slot.ItemDetails.ItemInstanceId,
			Slot.Count,
			Index);
	}
}
void UInventoryComponent::ForeachItem(TFunction<void(FName, int32, int32, int32)> const& Callback) const
{
	for (int32 Index = 0; Index < MaxItemCount; ++Index)
	{
		FItemRepresentation const& Slot = Items[Index];

		if (not Slot.IsEmpty())
		{
			Callback(
				Slot.ItemDetails.ItemClassId,
				Slot.ItemDetails.ItemInstanceId,
				Slot.Count,
				Index);
		}
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.Reserve(MaxItemCount);
	Items.AddZeroed(MaxItemCount);

#if WITH_EDITOR

	int32 Index = 0;
	for (FDataTableRowHandle const& Row : DesignTimeItems)
	{
		if (Index >= MaxItemCount)
		{
			break;
		}

		FItemDataRow*		 RowData = Row.GetRow<FItemDataRow>("DesignTime");
		FItemRepresentation& Item = Items[Index++];
		Item.Count = 1;
		Item.ItemDetails.ItemClassId = RowData->ID.GetTagName();
	}
#endif

	bIsInitialized = true;
	OnInitialized.Broadcast();
}
void UInventoryComponent::EndPlay(EEndPlayReason::Type const EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
void UInventoryComponent::ResetSlot(TArray<FItemRepresentation>::SizeType Index)
{
	Items[0] = {};
}
