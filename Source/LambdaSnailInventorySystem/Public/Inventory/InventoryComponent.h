#pragma once

#include "CoreMinimal.h"
#include "ItemDetails.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class IItemInterface;

UENUM()
enum struct EAddItemResult : uint8
{
	Success,
	InventoryFull
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	EAddItemResult AddItemToInventory(TScriptInterface<IItemInterface> Item);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetItemCount() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsFull() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetMaxItemCount() const;
	UFUNCTION(BlueprintCallable)
	void SetMaxItemCount(int32 NewCount);

	void ForeachItem(TFunction<void(int32 ItemClassID, int32 ItemInstanceID)> Callback) const;

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
	};

	UPROPERTY(EditAnywhere)
	int32 MaxItemCount{ 10 };

	TArray<FItemRepresentation> Items{};
};
