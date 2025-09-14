// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;

/**
 *
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitInventoryUI(class UInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void PostNativeInit();

protected:
	UPROPERTY(BlueprintReadonly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> OwningComponent;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ReloadInventory();
};
