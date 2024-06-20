// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory\ItemBase.h"

#include "Inventory/Inventory.h"
#include "Inventory/Actions/InventoryAction.h"

UItemBase::UItemBase()
{
	
}

bool UItemBase::IsSameItem(UItemBase const& Other) const
{
	return ItemMetadata->GetPrimaryAssetId() == Other.ItemMetadata->GetPrimaryAssetId();
}

void UItemBase::ChangeOwner(AActor* NewOwner)
{
	Rename(nullptr, NewOwner);
}

UItemType* UItemBase::GetItemData_Implementation() const
{
	return ItemMetadata;
}

void UItemBase::OnPickup_Implementation(AActor* Owner)
{
}

void UItemBase::OnDrop_Implementation(AActor* Owner)
{
}



void UItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add BP properties
	if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

TArray<TObjectPtr<UInventoryAction>> UItemBase::GetItemActions()
{
	if(ItemActionsCache.Num() > 0)
	{
		return ItemActionsCache;
	}

	for(auto ItemDefinition : ItemMetadata->InventoryActionDefinitions)
	{
		UInventoryAction* NewAction = NewObject<UInventoryAction>(this->GetOwner(), ItemDefinition.Get());
		NewAction->SetItem(this);
		ItemActionsCache.Add(NewAction);
	}

	return ItemActionsCache;
}
