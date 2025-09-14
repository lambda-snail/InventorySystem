// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Blueprints/AsyncAction_IterateInventory.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryOwner.h"
UAsyncAction_IterateInventory* UAsyncAction_IterateInventory::ForEachItem(UObject const* WorldContextObject, TScriptInterface<IInventoryOwner> Owner, bool ShouldIncludeEmptySlots)
{
	if (not GEngine)
	{
		return nullptr;
	}

	if (UWorld const* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		UAsyncAction_IterateInventory* Node = NewObject<UAsyncAction_IterateInventory>();
		Node->InventoryComponent = IInventoryOwner::Execute_GetInventory(Owner.GetObject());

		Node->RegisterWithGameInstance(World);
		return Node;
	}

	return nullptr;
}

void UAsyncAction_IterateInventory::Activate()
{
	auto Callback = [this](FName const ItemClassID, int32 const ItemInstanceID, int32 const Count, int32 const Index) {
		ItemIterator.Broadcast(ItemClassID, ItemInstanceID, Count, Index);
	};

	if (bShouldIncludeEmptySlots)
	{
		InventoryComponent->ForeachSlot(Callback);
	}
	else
	{
		InventoryComponent->ForeachItem(Callback);
	}

	OnIterationComplete.Broadcast();

	SetReadyToDestroy();
}