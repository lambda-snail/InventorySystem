// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemBase;
class UInventoryComponent;
// TODO: add type of change as parameter?
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryInitializedDelegate);

UENUM()
enum class EItemAddResult : uint8
{
	Success,
	SlotOccupied,
	InventoryFull,
	ItemTypeMismatch,
};

UENUM()
enum class EItemTransferResult
{
	/**
	 * The item was successfully transferred to the target slot. 
	 */
	Success,

	/**
	 * The target slot is occupied by an item of a different type.
	 */
	TargetSlotOccupied,

	/**
	 * The target slot is occupied and the items are of the same type, but the item does not stack.
	 */
	ItemNotStackable,

	/**
	 * An attempt was made to transfer an item from an empty slot.
	 */
	SourceIndexEmpty,

	/**
	 * An attempt was made to transfer an item from one slot to the same slot.
	 */
	NoTargetIndex,

	/**
	 * An attempt was made to move an item to a slot with an incompatible tag.  
	 */
	ItemTypeMismatch,

	/**
	 * An attempt was made to transfer a stack to a slot that does not stack.
	 */
	UnableToStack = 64
};

USTRUCT()
struct FItemTransferRequest
{
	GENERATED_BODY()

	uint32 SourceIndex;
	TObjectPtr<UInventoryComponent> SourceInventoryComponent;
	
	uint32 TargetIndex;
	TObjectPtr<UInventoryComponent> TargetInventoryComponent { nullptr };
};

UCLASS()
class UInventoryEntry : public UObject
{
	GENERATED_BODY()

public:
	TObjectPtr<UItemBase> Item;
	uint32 Index { 0 };
	int Count { 0 };
};

USTRUCT()
struct FItemSlotInstance
{
	GENERATED_BODY()
	
	TObjectPtr<UItemBase> Item;
	uint32 Index { 0 };
	
	int Count { 0 };

	// Determines which items are compatible with this slot. If the tag is empty, all items are allowed.
	FGameplayTag SlotTag;

	void SetData(TObjectPtr<UItemBase> NewItem, int ItemCount = 1)
	{
		Item = NewItem;
		Count = ItemCount;
	}
	
	void ResetData()
	{
		Item = nullptr;
		Count = 0;
	}

	bool IsEmpty() const
	{
		return not Item || Count == 0;
	}
};

UCLASS(ClassGroup=Inventory, hidecategories=(Object,LOD,Lighting,Transform,Sockets,TextureStreaming), editinlinenew, meta=(BlueprintSpawnableComponent))
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	uint32 GetCapacity() const;
	uint32 GetItemCount() const;
	
	// Broadcast changes in the inventory
	FOnInventoryChangedDelegate OnInventoryChanged;

	// Called when the inventory is finished initializing
	UPROPERTY(BlueprintAssignable)
	FOnInventoryInitializedDelegate OnInventoryInitialized;

	bool IsInitialized() const;

	UFUNCTION(BlueprintCallable)
	virtual EItemAddResult TryAddItem(UItemBase* Item);
	virtual EItemAddResult TryAddItem(UItemBase* Item, uint32 Slot);

	// TODO: When moving a stack to equipment it gets destroyed, so change this to virtual and override 
	virtual EItemTransferResult TryMoveItem(FItemTransferRequest const& TransferRequest);

	// Searches the inventory for an item instance matching a specific tag. Warning: May iterate over all slots in the container.
	TObjectPtr<UItemBase> GetItemByTag(FGameplayTag const ItemTag) const;
	TObjectPtr<UItemBase> GetItemInSlot(uint32 Slot) const;

	int32 GetInventoryCapacity() const;
	int32 GetNumItemsInSlot(uint32 Slot) const;
	
	/**
	 * Attempts to remove the item at the indicated slot. Will broadcast OnInventoryChanged and call OnDrop on the item.
	 */
	bool TryRemoveItem(uint32 Slot);

	TArray<UInventoryEntry*> GetItems() const;
	void ForEachItemInstance(TFunction<void(UItemBase const&, uint32 const Count, uint32 const Slot)> const& Callback) const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TArray<FItemSlotInstance> Items;

	// Inventory capacity is the number of items the inventory can hold
	UPROPERTY(EditAnywhere)
	uint32 InventoryCapacity { 10 };

	uint32 ItemCount { 0 };

	bool bIsInitialized { false };
};
