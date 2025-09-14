// Copyright LambdaSnail. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemDetails.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LAMBDASNAILINVENTORYSYSTEM_API IItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnAddedToInventory();

	UFUNCTION(BlueprintNativeEvent)
	void OnRemovedFromInventory();

	UFUNCTION(BlueprintNativeEvent)
	FItemDetails GetItemDetails() const;
	UFUNCTION(BlueprintNativeEvent)
	void SetItemDetails(FItemDetails ItemDetails);
	UFUNCTION(BlueprintNativeEvent)
	void ClearItemDetails();
};
