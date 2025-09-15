// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Widgets/InventoryWidget.h"

#include "Components/UniformGridPanel.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryGridCellInterface.h"
#include "Inventory/InventorySubsystem.h"
#include "Inventory/Logging/InventoryLogging.h"
#include "Inventory/Settings/LambdaSnail_InventorySettings.h"

void UInventoryWidget::InitInventoryUI(UInventoryComponent* InventoryComponent)
{
	OwningComponent = InventoryComponent;

	if (OwningComponent->IsInitialized())
	{
		InitWidgetInternal();
	}
	else
	{
		OwningComponent->OnInitialized.AddUniqueDynamic(this, &UInventoryWidget::InitWidgetInternal);
	}
}

void UInventoryWidget::ReloadInventory()
{
	ULambdaSnail_InventorySettings const* Settings = GetDefault<ULambdaSnail_InventorySettings>();
	TSubclassOf<UUserWidget>			  CellWidget = Settings->InventoryCellClass;

	OwningComponent->ForeachSlot(
		[this, CellWidget](FName ItemClassID, int32 ItemInstanceID, int32 Count, int32 Index) {
			UUserWidget* Cell = CreateWidget(GetOwningPlayer(), CellWidget);

			if (Cell->Implements<UInventoryGridCellInterface>())
			{
				IInventoryGridCellInterface::Execute_InitCell(Cell, ItemClassID, ItemInstanceID, Count, Index);
			}
			else
			{
				UE_LOGFMT(LogInventory, Error, "Widget does not implement item cell interface: {Object}", Cell->GetName());
			}

			int32 Row = Index / GridDimensions.X;
			int32 Column = Index % GridDimensions.X;

			Grid->AddChildToUniformGrid(Cell, Row, Column);
		});
}

void UInventoryWidget::InitWidgetInternal()
{
	OwningComponent->OnInitialized.RemoveAll(this);
	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetOwningPlayer());
	if (InventorySubsystem->IsInitialized())
	{
		UE_LOGFMT(LogInventory, Display, "Initializing inventory widget {WidgetName}", this->GetName());
		InventorySubsystem->OnSubsystemInitialized.RemoveAll(this);
		InitWidget();
		ReloadInventory();
	}
	else
	{
		InventorySubsystem->OnSubsystemInitialized.AddDynamic(this, &ThisClass::InitWidgetInternal);
	}
}
