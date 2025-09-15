// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
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

	/** Called by the native code when the widget is ready to initialize blueprint customizations. ReloadInventory is called automatically after this. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void InitWidget();

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FIntVector2 GridDimensions{ 5, 10 };

protected:
	UPROPERTY(BlueprintReadonly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> OwningComponent;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ReloadInventory();

	UFUNCTION()
	void InitWidgetInternal();
};
