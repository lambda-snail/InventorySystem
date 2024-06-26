﻿#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Inventory.generated.h"

class UInventoryAction;

/**
 * Data asset that describes things common to all inventory items of a certain type,
 * such as name, icon etc.
 */
UCLASS(BlueprintType)
class LAMBDASNAILINVENTORYSYSTEM_API UItemType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	FPrimaryAssetId GetPrimaryAssetId() const override { return FPrimaryAssetId("AssetItems", GetFName()); } 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="UI")
	FSlateBrush Icon;

	// Determines if the item can stack or not
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Behavior")
	bool bIsStackable { true };

	// Determines how many items can be in one stack, if bIsStackable is true
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Behavior", meta = (EditConditionHides=bIsStackable))
	int MaxStackSize { 10 };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Behavior")
	FGameplayTagContainer ItemTags;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UInventoryAction>> InventoryActionDefinitions;
};
