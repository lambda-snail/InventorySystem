// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UItemSlotInstance;
class UListView;
class UWrapBox;
class UTileView;
class UUniformGridPanel;
class UItemWidget;

/**
 * 
 */
UCLASS(Abstract)
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UInventoryComponent> OwningComponent;

	/**
	 * Sets the inventory this widget should listen to, and initializes the widget. If the inventory component is not
	 * already initialized a callback will be registered on InventoryComponent.OnInventoryInitialized to defer the
	 * initialization until after the inventory is set up.
	 */
	UFUNCTION(BlueprintCallable)
	void SetOwnerAndInitialize(UInventoryComponent* InventoryComponent);
	
	UFUNCTION(BlueprintCallable)
	virtual void Init();
protected:
	virtual void NativeConstruct() override;

	// The number of columns in the inventory grid
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// int NumColumns { 5 };
	
	TArray<UItemSlotInstance*> Items;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> InventoryView;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TSubclassOf<UItemWidget> GridItemWidgetClass;
private:

	UFUNCTION()
	void ReloadInventory();
};
