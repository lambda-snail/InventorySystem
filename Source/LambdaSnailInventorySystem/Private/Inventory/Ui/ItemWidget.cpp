// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Ui/ItemWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/StaticMeshActor.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemBase.h"
#include "Inventory/Ui/InventoryDragDropOperation.h"

void UItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if(UItemSlotInstance* Entry = Cast<UItemSlotInstance>(ListItemObject))
	{
		SetItem(Entry->GetItem(), Entry->Count);
	}
}

// TODO: Remove and move to native set
void UItemWidget::SetItem(UItemBase const* Item, int Count)
{
	this->ItemInstance = Item;
	
	DisplayCount->SetText(FText::AsNumber(Count));
	if(UItemType const* ItemData = Item->GetItemData())
	{
		Image->SetBrush(ItemData->Icon);
		DisplayName->SetText(ItemData->DisplayName);
	}

	bIsOccupied = true;
	OnSetItem();
}

void UItemWidget::UnsetItem()
{
	Icon = DefaultIcon;
	ItemInstance = nullptr;
	bIsOccupied = false;
	//DisplayName-> = FText::GetEmpty();
	//ItemCount = 0;
	OnUnsetItem();
}

void UItemWidget::RemoveAndSpawnItem()
{
	TObjectPtr<UItemBase> RemovedItem = InventoryComponent->GetItemInSlot(ItemIndex);
	
	if(InventoryComponent->TryRemoveItem(ItemIndex))
	{
		RemovedItem->OnDrop(InventoryComponent->GetOwner());
	}
}

void UItemWidget::SetInventoryComponent(UInventoryComponent* Inventory)
{
	InventoryComponent = Inventory;
}

void UItemWidget::SetItemIndex(uint32 Index)
{
	ItemIndex = Index;
}

bool UItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                               UDragDropOperation* InOperation)
{
	if(UInventoryDragDropOperation const* DragOperation = Cast<UInventoryDragDropOperation>(InOperation))
	{
		UItemWidget* SourceWidget = DragOperation->GetWidgetReference();
		
		// auto const LocalPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
		// FVector2D const DragOffset = DragOperation->GetDragOffset();
		// FVector2D Position = LocalPosition - DragOffset;
		
		FItemTransferRequest const TransferRequest {
			.SourceIndex = SourceWidget->ItemIndex,
			.SourceInventoryComponent = SourceWidget->InventoryComponent,
			.TargetIndex = this->ItemIndex,
			.TargetInventoryComponent = this->InventoryComponent
		};
		
		EItemTransferResult const Result = InventoryComponent->TryMoveItem(TransferRequest);
		if(Result != EItemTransferResult::Success)
		{
			// TODO: Add feedback to underlying inventory system, should also be able to handle when dropped onto
            // a different inventory
		}
		
		return true;
	}

	return false;
}

void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
