// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Ui/InventoryWidget.h"

#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Inventory/Ui/ItemWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/WrapBox.h"
#include "Inventory/InventoryComponent.h"

void UInventoryWidget::SetOwnerAndInitialize(UInventoryComponent* InventoryComponent)
{
	OwningComponent = InventoryComponent;
	if(OwningComponent->IsInitialized())
	{
		Init();
	}
	else
	{
		OwningComponent->OnInventoryInitialized.AddUniqueDynamic(this, &UInventoryWidget::Init);
	}
}

void UInventoryWidget::TileView_OnItemSelectionChange(UObject* Item)
{
	if(UItemSlotInstance* InventorySlot = Cast<UItemSlotInstance>(Item))
	{
		if(InventorySlot->GetItem())
		{
			InventoryActionsGrid->SetListItems<TObjectPtr<UInventoryAction>>(InventorySlot->GetItem()->GetItemActions());	
		}
	}
}

void UInventoryWidget::Init()
{
	if(OwningComponent)
	{
		//Items = OwningComponent->GetItems();
		//
		//
		//
		// for(uint32 i = 0; i < OwningComponent->GetCapacity(); ++i)
		// {
		// 	UItemWidget* ItemWidget = CreateWidget<UItemWidget>(this, GridItemWidgetClass);
		// 	ItemWidget->UnsetItem();
		// 	Items.Add(ItemWidget);
		//
		// 	ItemWidget->SetItemIndex(i);
		// 	ItemWidget->SetInventoryComponent(OwningComponent);
		//
		// 	uint32 const Column	= i % static_cast<uint32>(NumColumns); 
		// 	uint32 const Row	= i / static_cast<uint32>(NumColumns);
		// 	ItemView->AddChildToUniformGrid(ItemWidget, Row, Column);
		// }
		
		ReloadInventory();
		
		OwningComponent->OnInventoryChanged.AddDynamic(this, &UInventoryWidget::ReloadInventory);
		OwningComponent->OnInventoryInitialized.RemoveDynamic(this, &UInventoryWidget::Init);

		InventoryView->OnItemSelectionChanged().AddUObject(this, &ThisClass::TileView_OnItemSelectionChange);
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::ReloadInventory()
{
	Items = OwningComponent->GetItems();
	InventoryView->SetListItems<UItemSlotInstance*>(Items);

	// for(auto* Item : Items)
	// {
	// 	Item->UnsetItem();
	// }
	//
	// OwningComponent->ForEachItemInstance([this](UItemBase const& ItemInstance, uint32 const Count, uint32 const SlotIndex)
	// {
	// 	auto* ItemWidget = Items[SlotIndex];
	// 	ItemWidget->SetItem(ItemInstance, Count);
	// });
}
