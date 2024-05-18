// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryActor.generated.h"

class UInventoryComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryActor : public UInterface
{
	GENERATED_BODY()
};

class LAMBDASNAILINVENTORYSYSTEM_API IInventoryActor
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual UInventoryComponent* GetInventoryComponent() const = 0;
};
