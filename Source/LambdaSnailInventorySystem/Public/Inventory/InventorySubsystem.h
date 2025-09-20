// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include <atomic>

#include "InventorySubsystem.generated.h"

class UItemDataAsset;
struct FItemDataRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSubsystemInitializedDelegate);

/**
 *
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UInventorySubsystem* Get(UObject const* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LambdaSnail|Inventory")
	TSoftObjectPtr<UItemDataAsset> GetItemClassData(FGameplayTag const ItemID) const;

	//~Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End USubsystem Interface

	UPROPERTY(BlueprintAssignable)
	FOnSubsystemInitializedDelegate OnSubsystemInitialized;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInitialized() const { return bIsInitialized.load(); }

private:
	FGameplayTagContainer ItemTags{};

	TMap<FGameplayTag, TSoftObjectPtr<UItemDataAsset>> ItemDataMap;

	std::atomic<bool> bIsInitialized{ false };
};
