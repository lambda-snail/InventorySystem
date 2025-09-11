// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InstancedItem.generated.h"

class UInventoryComponent;

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UInstancedItem : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LAMBDASNAILINVENTORYSYSTEM_API IInstancedItem
{
	GENERATED_BODY()

public:
	uint32 GetInstanceId();
	void   SetInstanceId(uint32 Id);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEnterInventory(UInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLeavingInventory(UInventoryComponent* InventoryComponent);
};
