#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryOwner.generated.h"

class UInventoryComponent;

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UInventoryOwner : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LAMBDASNAILINVENTORYSYSTEM_API IInventoryOwner
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryComponent* GetInventory() const;
};
