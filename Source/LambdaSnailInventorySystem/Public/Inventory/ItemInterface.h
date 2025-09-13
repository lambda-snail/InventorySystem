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
	UFUNCTION(BlueprintImplementableEvent)
	void OnAddedToInventory();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRemovedFromInventory();

	FItemDetails GetItemDetails() const;
	void		 SetItemDetails(FItemDetails ItemDetails);
	void		 ClearItemDetails();
};
