// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory\ItemBase.h"

#include "Inventory/Inventory.h"

UItemBase::UItemBase()
{
	
}

bool UItemBase::IsSameItem(UItemBase const& Other) const
{
	return ItemMetadata->GetPrimaryAssetId() == Other.ItemMetadata->GetPrimaryAssetId();
}

void UItemBase::CreateComponentForPickup(AActor* Actor) const
{
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
