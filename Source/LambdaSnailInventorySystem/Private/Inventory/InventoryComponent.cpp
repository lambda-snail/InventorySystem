// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryComponent.h"

#include "Engine/ActorChannel.h"
#include "Inventory/ItemBase.h"
#include "Net/UnrealNetwork.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bReplicateUsingRegisteredSubObjectList = true;
	//SetIsReplicated(true);
}

void UInventoryComponent::BeginPlay()
{
	if(GetOwner()->HasAuthority())
	{
		Items.Reserve(InventoryCapacity);
		for (uint32 i = 0; i < InventoryCapacity; ++i)
		{
			UItemSlotInstance* Slot = NewObject<UItemSlotInstance>(GetOwner(), UItemSlotInstance::StaticClass());
			Slot->SetData(nullptr, 0);
			Slot->Index = i;
		
			Items.Add(Slot);
			AddReplicatedSubObject(Slot);
		}	
	}
	
	Super::BeginPlay();
	bIsInitialized = true;

	OnInventoryInitialized.Broadcast();
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for(UItemSlotInstance* Slot : Items)
	{
		RemoveReplicatedSubObject(Slot);
	}
	
	Super::EndPlay(EndPlayReason);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Items);
	DOREPLIFETIME(ThisClass, InventoryCapacity);
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
			if (Items[i]->IsEmpty())
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

	if (Items[Slot]->SlotTag.IsValid() && not Item->GetItemData()->ItemTags.HasTag(Items[Slot]->SlotTag))
	{
		return EItemAddResult::ItemTypeMismatch;
	}

	if (Items[Slot]->IsEmpty())
	{
		Items[Slot]->SetData(Item, 1);
		++ItemCount;

		AddReplicatedSubObject(Item);
		
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
	
	auto& SourceItem = *SourceSlot->GetItem();

	// Check that we are not accidentally transferring from an empty slot
	if (SourceSlot->IsEmpty())
	{
		return EItemTransferResult::SourceIndexEmpty;
	}

	// Are we trying to move to the same slot for some reason?
	if (SourceInventory == TargetInventory && TransferRequest.SourceIndex == TransferRequest.TargetIndex)
	{
		return EItemTransferResult::NoTargetIndex;
	}

	// Does the target slot have the correct type to accomodate the source item?
	if (TargetSlot->SlotTag.IsValid() && not SourceItem.GetItemData()->ItemTags.HasTag(TargetSlot->SlotTag))
	{
		return EItemTransferResult::ItemTypeMismatch;
	}

	// Target slot empty and all checks ok - move the item
	if (TargetSlot->IsEmpty())
	{
		TargetSlot->SetData(&SourceItem, 1);
		SourceSlot->ResetData();
		
		SourceInventory->OnInventoryChanged.Broadcast();
		TargetInventory->OnInventoryChanged.Broadcast();
		return EItemTransferResult::Success;
	}

	// Target slot occupied
	auto& TargetItem = *TargetSlot->GetItem();
	
	// -  Different items
	if (not SourceItem.IsSameItem(TargetItem))
	{
		return EItemTransferResult::TargetSlotOccupied;
	}

	// -  Same type and stackable
	if (SourceItem.GetItemData()->bIsStackable)
	{
		TargetSlot->Count += SourceSlot->Count;

		SourceSlot->ResetData();

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
		if (Items[i]->GetItem()->GetItemData()->ItemTags.HasTag(ItemTag))
		{
			return Items[i]->GetItem();
		}
	}

	return nullptr;
}

TObjectPtr<UItemBase> UInventoryComponent::GetItemInSlot(uint32 Slot) const
{
	return Items[Slot]->GetItem();
}

int32 UInventoryComponent::GetInventoryCapacity() const
{
	return InventoryCapacity;
}

int32 UInventoryComponent::GetNumItemsInSlot(uint32 Slot) const
{
	return Items[Slot]->Count;
}

bool UInventoryComponent::TryRemoveItem(uint32 Slot)
{
	if (Slot < InventoryCapacity && Items[Slot]->Count > 0)
	{
		UItemBase* Item = Items[Slot]->GetItem();
		
		Items[Slot]->ResetData();
		OnInventoryChanged.Broadcast();
		
		Item->OnDrop(GetOwner());
		
		return true;
	}

	return false;
}

TArray<TObjectPtr<UItemSlotInstance>> UInventoryComponent::GetItems() const
{
	// TArray<UItemSlotInstance*> Entries{};
	// Entries.Reserve(Items.Num());
	//
	// for (int i = 0; i < Items.Num(); ++i)
	// {
	// 	if(Items[i] and Items[i]->Item)
	// 	{
	// 		// TODO: Make slot uobject with an interface so we don't have to allocate?
	// 		auto Entry = NewObject<UItemSlotInstance>();
	// 		Entry->Item = Items[i]->Item;
	// 		Entry->Index = Items[i]->Index;
	// 		Entry->Count = Items[i]->Count;
	// 	
	// 		Entries.Add(Entry);			
	// 	}
	// }

	TArray<TObjectPtr<UItemSlotInstance>> NonEmptyItems;
	for (UItemSlotInstance* Slot : Items)
	{
		if(Slot and Slot->GetItem())
		{
			NonEmptyItems.Add(Slot);
		}
	}
	
	return 	NonEmptyItems;
}

void UInventoryComponent::ForEachItemInstance(
	TFunction<void(UItemBase const&, uint32 const Count, uint32 const Slot)> const& Callback) const
{
	for(uint32 i = 0; i < InventoryCapacity; ++i)
	{
		UItemSlotInstance const* ItemSlot{ Items[i] };
		if (not ItemSlot->IsEmpty())
		{
			Callback(*ItemSlot->GetItem(), ItemSlot->Count, ItemSlot->Index);
		}
	}
}

void UInventoryComponent::OnRep_Items()
{
	ItemCount = 0;
	for(auto const& Slot : Items)
	{
		if(Slot)
		{
			ItemCount += Slot->IsEmpty() ? 0 : 1;	
		}
	}

	if(OnInventoryChanged.IsBound())
	{
		OnInventoryChanged.Broadcast();
	}
}

void UInventoryComponent::OnRep_InventoryCapacity()
{
	if(OnInventoryChanged.IsBound())
	{
		OnInventoryChanged.Broadcast();
	}
}






void UItemSlotInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Item);
	DOREPLIFETIME(ThisClass, Count);

	DOREPLIFETIME_CONDITION(ThisClass, Index, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ThisClass, SlotTag, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ThisClass, OwningInventoryComponent, COND_InitialOnly);
}

void UItemSlotInstance::OnRep_Item()
{
	GEngine->AddOnScreenDebugMessage(23, 2.f, FColor::Red, "Item Replicated!");
}
