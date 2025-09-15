// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataTable.h"
#include "LambdaSnail_InventorySettings.generated.h"

/**
 *
 */
UCLASS(Config = "LambdaSnail", DefaultConfig, meta = (DisplayName = "Inventory Settings"))
class LAMBDASNAILINVENTORYSYSTEM_API ULambdaSnail_InventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	explicit ULambdaSnail_InventorySettings(FObjectInitializer const& Initializer);

	UPROPERTY(Config, EditAnywhere, Category = "LambdaSnail|Inventory", meta = (ForceInlineRow, AllowedClasses = "/Script/Engine.DataTable", RequiredAssetDataTags = "RowStructure=/Script/LambdaSnailInventorySystem.ItemDataRow"))
	FSoftObjectPath InventoryTable;

	UPROPERTY(Config, EditAnywhere, Category = "LambdaSnail|Inventory", meta = (MustImplement = "InventoryGridCellInterface"))
	TSubclassOf<class UUserWidget> InventoryCellClass;
};
