// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/Inventory.h"
#include "Templates/SharedPointer.h"
#include "ItemWidget.generated.h"

class UTextBlock;
class UItemBase;
class UInventoryComponent;
class UImage;
class UItemType;

/**
 * Represents a single item in an inventory grid. 
 */
UCLASS(Abstract)
class LAMBDASNAILINVENTORYSYSTEM_API UItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	void SetItem(UItemBase* Item, int Count);
	void UnsetItem();

	/**
	 * Attempt to remove the underlying item from the inventory and spawn the corresponding item in the world.
	 * TODO: Should probably be implemented in a controller class
	 */
	void RemoveAndSpawnItem();

	void SetInventoryComponent(UInventoryComponent* Inventory);
	void SetItemIndex(uint32 Index);
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
protected:
	TObjectPtr<UItemBase> ItemInstance;

	bool bIsOccupied { false };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateBrush Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateBrush DefaultIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayCount;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(BlueprintReadOnly);
	TArray<UInventoryAction*> InventoryActions;
	
	uint32 ItemIndex;
	
	// Called after an item has been added to the slot
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetItem();

	// Called after an item has been removed from the slot
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnsetItem();
};
