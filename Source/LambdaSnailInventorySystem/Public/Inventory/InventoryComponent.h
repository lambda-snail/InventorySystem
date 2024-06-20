// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "ItemBase.h"
#include "Network/SubobjectReplicator.h"
#include "Components/ActorComponent.h"
#include "Network/ReplicatedObject.h"
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

/**
 * Internally items in the inventory are stored in an FItemSlotInstance that keeps track the item and metadata related
 * to storing that item.
 */
UCLASS(BlueprintType)
class UItemSlotInstance : public UReplicatedObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	uint32 Index { 0 };

	UPROPERTY(Replicated)
	int Count { 0 };

	// Determines which items are compatible with this slot. If the tag is empty, all items are allowed.
	UPROPERTY(Replicated)
	FGameplayTag SlotTag;
	
	ISubobjectReplicator* ParentReplicator;

	void SetData(UItemBase* NewItem, int ItemCount = 1);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UItemBase* GetItem() const { return Item; };
	
	void ResetData()
	{
		if(ParentReplicator and GetOwner()->HasAuthority() and Item)
		{
			ParentReplicator->RemoveReplicatedObject(Item);
		}
		
		Item = nullptr;
		Count = 0;
	}

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const
	{
		return not Item || Count == 0;
	}

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE virtual void BeginDestroy() override
	{
		if(ParentReplicator and GetOwner()->HasAuthority() and Item)
		{
			ParentReplicator->RemoveReplicatedObject(Item);
		}

		Super::BeginDestroy();
	}
	
	UPROPERTY(ReplicatedUsing=OnRep_Item)
	TObjectPtr<UItemBase> Item;
	UFUNCTION()
	void OnRep_Item();
};

UCLASS(ClassGroup=Inventory, hidecategories=(Object,LOD,Lighting,Transform,Sockets,TextureStreaming), editinlinenew, meta=(BlueprintSpawnableComponent))
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent, public ISubobjectReplicator
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

	UFUNCTION(BlueprintCallable)
	bool IsInitialized() const;

	UFUNCTION(BlueprintCallable)
	virtual EItemAddResult TryAddItem(UItemBase* Item);
	virtual EItemAddResult TryAddItem(UItemBase* Item, uint32 Slot);

	// RPC Server
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

	TArray<TObjectPtr<UItemSlotInstance>> GetItems() const;
	void ForEachItemInstance(TFunction<void(UItemBase const&, uint32 const Count, uint32 const Slot)> const& Callback) const;

	virtual void AddReplicatedObject(UReplicatedObject* Object) override;
	virtual void RemoveReplicatedObject(UReplicatedObject* Object) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Items)
	TArray<TObjectPtr<UItemSlotInstance>> Items;

	// Inventory capacity is the number of items the inventory can hold
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_InventoryCapacity)
	uint32 InventoryCapacity { 10 };

	UPROPERTY(Replicated)
	uint32 ItemCount { 0 };

	bool bIsInitialized { false };
	
private:
	UFUNCTION()
	void OnRep_Items();
	UFUNCTION()
	void OnRep_InventoryCapacity();

	friend class UItemSlotInstance;
};
