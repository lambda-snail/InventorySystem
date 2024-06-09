// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Items.Reserve(InventoryCapacity);
	for (uint32 i = 0; i < InventoryCapacity; ++i)
	{
		FItemSlotInstance Slot{};
		Slot.Item = nullptr;
		Slot.Index = i;
		
		Items.Add(Slot);
	}
	
	Super::BeginPlay();
	bIsInitialized = true;

	OnInventoryInitialized.Broadcast();
}

uint32 UInventoryComponent::GetCapacity() const
{
	return InventoryCapacity;
}

uint32 UInventoryComponent::GetItemCount() const
{
	return ItemCount;
}

bool UInventoryComponent::IsInitialized() const
{
	return bIsInitialized;
}

// TODO: Take TObjectPtr<UItemType> instead
// TODO: Allow to choose slot? Or should inventory stack automatically? Or should the player be able to choose behavior?
EItemAddResult UInventoryComponent::TryAddItem(UItemBase* Item)
{
	if (ItemCount < InventoryCapacity)
	{
		for (uint32 i = 0; i < InventoryCapacity; ++i)
		{
			if (Items[i].IsEmpty())
			{
				return TryAddItem(Item, i);
			}
		}
	}

	return EItemAddResult::InventoryFull;
}

EItemAddResult UInventoryComponent::TryAddItem(UItemBase* Item, uint32 Slot)
{
	verify(Slot < static_cast<uint32>(Items.Num()));

	if (Items[Slot].SlotTag.IsValid() && not Item->GetItemData()->ItemTags.HasTag(Items[Slot].SlotTag))
	{
		return EItemAddResult::ItemTypeMismatch;
	}

	if (Items[Slot].IsEmpty())
	{
		Items[Slot].SetData(Item, 1);
		++ItemCount;

		OnInventoryChanged.Broadcast();
		return EItemAddResult::Success;
	}
	// TODO: Check item type and increment
	else
	{
		return EItemAddResult::SlotOccupied;
	}
}

EItemTransferResult UInventoryComponent::TryMoveItem(FItemTransferRequest const& TransferRequest)
{
	auto& SourceInventory = TransferRequest.SourceInventoryComponent;
	auto& TargetInventory = TransferRequest.TargetInventoryComponent;

	verify(TransferRequest.SourceIndex < SourceInventory->InventoryCapacity);
	verify(TransferRequest.TargetIndex < TargetInventory->InventoryCapacity);
	
	auto& SourceSlot = SourceInventory->Items[TransferRequest.SourceIndex];
	auto& TargetSlot = TargetInventory->Items[TransferRequest.TargetIndex];
	
	auto& SourceItem = *SourceSlot.Item;

	// Check that we are not accidentally transferring from an empty slot
	if (SourceSlot.IsEmpty())
	{
		return EItemTransferResult::SourceIndexEmpty;
	}

	// Are we trying to move to the same slot for some reason?
	if (SourceInventory == TargetInventory && TransferRequest.SourceIndex == TransferRequest.TargetIndex)
	{
		return EItemTransferResult::NoTargetIndex;
	}

	// Does the target slot have the correct type to accomodate the source item?
	if (TargetSlot.SlotTag.IsValid() && not SourceItem.GetItemData()->ItemTags.HasTag(TargetSlot.SlotTag))
	{
		return EItemTransferResult::ItemTypeMismatch;
	}

	// Target slot empty and all checks ok - move the item
	if (TargetSlot.IsEmpty())
	{
		TargetSlot.SetData(&SourceItem, 1);

		SourceSlot.ResetData();

		SourceInventory->OnInventoryChanged.Broadcast();
		TargetInventory->OnInventoryChanged.Broadcast();
		return EItemTransferResult::Success;
	}

	// Target slot occupied
	auto& TargetItem = *TargetSlot.Item;
	
	// -  Different items
	if (not SourceItem.IsSameItem(TargetItem))
	{
		return EItemTransferResult::TargetSlotOccupied;
	}

	// -  Same type and stackable
	if (SourceItem.GetItemData()->bIsStackable)
	{
		TargetSlot.Count += SourceSlot.Count;

		SourceSlot.ResetData();

		SourceInventory->OnInventoryChanged.Broadcast();
		TargetInventory->OnInventoryChanged.Broadcast();
		return EItemTransferResult::Success;
	}

	// -  Same type not stackable
	return EItemTransferResult::ItemNotStackable;
}

TObjectPtr<UItemBase> UInventoryComponent::GetItemByTag(FGameplayTag const ItemTag) const
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].Item->GetItemData()->ItemTags.HasTag(ItemTag))
		{
			return Items[i].Item;
		}
	}

	return nullptr;
}

TObjectPtr<UItemBase> UInventoryComponent::GetItemInSlot(uint32 Slot) const
{
	return Items[Slot].Item;
}

int32 UInventoryComponent::GetInventoryCapacity() const
{
	return InventoryCapacity;
}

int32 UInventoryComponent::GetNumItemsInSlot(uint32 Slot) const
{
	return Items[Slot].Count;
}

bool UInventoryComponent::TryRemoveItem(uint32 Slot)
{
	if (Slot < InventoryCapacity && Items[Slot].Count > 0)
	{
		UItemBase* Item = Items[Slot].Item;
		
		Items[Slot].ResetData();
		OnInventoryChanged.Broadcast();
		
		Item->OnDrop(GetOwner());
		
		return true;
	}

	return false;
}

TArray<UInventoryEntry*> UInventoryComponent::GetItems() const
{
	TArray<UInventoryEntry*> Entries{};
	Entries.Reserve(Items.Num());

	for (int i = 0; i < Items.Num(); ++i)
	{
		if(not Items[i].Item)
		{
			continue;
		}
		
		// TODO: Make slot uobject with an interface so we don't have to allocate?
		auto Entry = NewObject<UInventoryEntry>();
		Entry->Item = Items[i].Item;
		Entry->Index = Items[i].Index;
		Entry->Count = Items[i].Count;
		
		Entries.Add(Entry);
	}
	
	return 	Entries;
}

void UInventoryComponent::ForEachItemInstance(
	TFunction<void(UItemBase const&, uint32 const Count, uint32 const Slot)> const& Callback) const
{
	for(uint32 i = 0; i < InventoryCapacity; ++i)
	{
		FItemSlotInstance const& ItemSlot{ Items[i] };
		if (not ItemSlot.IsEmpty())
		{
			Callback(*ItemSlot.Item, ItemSlot.Count, ItemSlot.Index);
		}
	}
}
