// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Widgets/InventoryWidget.h"
#include "Inventory/InventoryComponent.h"

void UInventoryWidget::InitInventoryUI(UInventoryComponent* InventoryComponent)
{
	OwningComponent = InventoryComponent;

	// if(OwningComponent->IsInitialized())
	// {
	// 	Init();
	// }
	// else
	// {
	// 	OwningComponent->OnInventoryInitialized.AddUniqueDynamic(this, &UInventoryWidget::Init);
	// }
}

void UInventoryWidget::ReloadInventory()
{
	OwningComponent->ForeachSlot(
		[](FName ItemClassID, int32 ItemInstanceID, int32 Count, int32 Index) {
			// InventoryView->SetListItems<UItemSlotInstance*>(Items);
		});
}
