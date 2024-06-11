// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryActor.generated.h"

class UInventoryComponent;

UINTERFACE(MinimalAPI)
class UInventoryActor : public UInterface
{
	GENERATED_BODY()
};

class LAMBDASNAILINVENTORYSYSTEM_API IInventoryActor
{
	GENERATED_BODY()

public:
	virtual UInventoryComponent* GetInventoryComponent() const = 0;
};
