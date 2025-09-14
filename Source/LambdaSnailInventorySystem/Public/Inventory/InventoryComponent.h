#pragma once

#include "CoreMinimal.h"
#include "ItemDetails.h"
#include "Components/ActorComponent.h"
#include "Templates/Function.h"
#include "InventoryComponent.generated.h"

class IItemInterface;

UENUM()
enum struct EAddItemResult : uint8
{
	Success,
	InventoryFull,
	/** When adding an item to a slot, but the target slot is occupied by an item of incompatible type */
	ItemTypeMismatch,
	/** Attempted to add an item to a negative slot or a slot beyond the limits of the inventory */
	InvalidSlot
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAddedDelegate, FName, ItemClassId, int32, ItemInstanceId, int32, Index);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	EAddItemResult AddItemToInventory(TScriptInterface<IItemInterface> Item);

	UFUNCTION(BlueprintCallable)
	EAddItemResult AddItemToInventorySlot(TScriptInterface<IItemInterface> Item, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetItemCount() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsFull() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetMaxItemCount() const;
	UFUNCTION(BlueprintCallable)
	void SetMaxItemCount(int32 NewCount);

	/** Execute logic for each slot in the inventory. If Count == 0 the slot is empty. */
	void ForeachSlot(TFunction<void(FName, int32, int32, int32)> const& Callback) const;

	/** Execute logic for each item in the inventory. Skips empty slots, so the Index parameter is not guaranteed to be contiguous. */
	void ForeachItem(TFunction<void(FName, int32, int32, int32)> const& Callback) const;

	UPROPERTY(BlueprintAssignable)
	FOnItemAddedDelegate OnItemAdded;

protected:
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type const EndPlayReason) override;
	//~ End UActorComponent Interface

private:
	struct FItemRepresentation
	{
		FItemDetails ItemDetails{};
		uint8		 Count{};

		bool FORCEINLINE IsEmpty() const { return Count == 0; }
	};

	UPROPERTY(EditAnywhere)
	int32 MaxItemCount{ 10 };

	int32 CurrentItemCount{ 0 };

	TArray<FItemRepresentation> Items{};

	void FORCEINLINE ResetSlot(TArray<FItemRepresentation>::SizeType Index);
};
