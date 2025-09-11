// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventorySubsystem.generated.h"

/**
 *
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UInventorySubsystem* Get(UObject const* WorldContextObject);
};
