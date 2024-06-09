// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/EquipmentComponent.h"

void UEquipmentComponent::BeginPlay()
{
	InventoryCapacity = EquipmentSlotTypes.Num();

	Super::BeginPlay();
	
	for (int32 i = 0; i < EquipmentSlotTypes.Num(); ++i)
	{
		Items[i]->SlotTag = EquipmentSlotTypes[i];
	}
	
}

void UEquipmentComponent::ForEachEquipmentInstance(
	TFunction<void(UItemBase const&, FGameplayTag SlotTag, uint32 const Slot)> const& Callback) const
{
	for(uint32 i = InventoryCapacity; i < InventoryCapacity; ++i)
	{
		UItemSlotInstance const* ItemSlot{ Items[i] };
		if (not ItemSlot->IsEmpty())
		{
			Callback(*ItemSlot->GetItem(), ItemSlot->SlotTag, ItemSlot->Index);
		}
	}
}

EItemTransferResult UEquipmentComponent::TryMoveItem(FItemTransferRequest const& TransferRequest)
{
	if(TransferRequest.SourceInventoryComponent->GetNumItemsInSlot(TransferRequest.SourceIndex) > 1u)
	{
		return EItemTransferResult::UnableToStack;
	}
	
	return Super::TryMoveItem(TransferRequest);
}
