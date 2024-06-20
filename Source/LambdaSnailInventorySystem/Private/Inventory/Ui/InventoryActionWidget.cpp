// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Ui/InventoryActionWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Actions/InventoryAction.h"

void UInventoryActionWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if(UInventoryAction* InAction = Cast<UInventoryAction>(ListItemObject))
	{
		ActionName->SetText( InAction->Name );
		ActionIcon->SetBrush( InAction->Icon );
		Action = InAction;
	}
}
