#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "InventoryGridCellInterface.generated.h"

UINTERFACE()
class UInventoryGridCellInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LAMBDASNAILINVENTORYSYSTEM_API IInventoryGridCellInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "LambdaSnail|Inventory")
	void InitCell(FGameplayTag ItemClassID, int32 ItemInstanceID, int32 Count, int32 Index);

	UFUNCTION(BlueprintImplementableEvent, Category = "LambdaSnail|Inventory")
	void ClearCell();
};
