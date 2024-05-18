// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Ui/InventoryDragDropOperation.h"

UItemWidget* UInventoryDragDropOperation::GetWidgetReference() const
{
	return WidgetReference;
}

void UInventoryDragDropOperation::SetWidgetReference(UItemWidget* Widget)
{
	WidgetReference = Widget;
}

FVector2D UInventoryDragDropOperation::GetDragOffset() const
{
	return DragOffset;
}

void UInventoryDragDropOperation::SetDragOffset(FVector2D NewDragOffset)
{
	DragOffset = NewDragOffset;
}
