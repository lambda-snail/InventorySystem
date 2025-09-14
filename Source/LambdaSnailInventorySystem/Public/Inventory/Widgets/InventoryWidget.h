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
	UFUNCTION(BlueprintCallable)
	void InitInventoryUI(class UInventoryComponent* InventoryComponent);

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> OwningComponent;

	UFUNCTION(BlueprintCallable)
	void ReloadInventory();
};
