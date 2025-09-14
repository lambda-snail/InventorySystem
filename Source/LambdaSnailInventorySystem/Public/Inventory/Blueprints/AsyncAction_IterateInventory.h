// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_IterateInventory.generated.h"

class IInventoryOwner;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FItemIterator, FName, ItemClassID, int32, ItemInstanceID, int32, Count, int32, Index);

/**
 *
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API UAsyncAction_IterateInventory : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Iterate Over Items", Category = "Lambda Snail Blueprints"))
	static UAsyncAction_IterateInventory* ForEachItem(
		UObject const*					  WorldContextObject,
		TScriptInterface<IInventoryOwner> Owner,
		bool							  ShouldIncludeEmptySlots);

	//~ Begin UBlueprintAsyncActionBase Interface
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface

	UPROPERTY(BlueprintAssignable)
	FItemIterator ItemIterator;

private:
	UPROPERTY(Transient)
	TObjectPtr<UInventoryComponent> InventoryComponent;
	bool							bShouldIncludeEmptySlots{ false };
};
