// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include <atomic>

#include "InventorySubsystem.generated.h"

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
	void GetItemClassData(FName ItemID, FItemDataRow& OutDataRow, bool& Success) const;

	FItemDataRow* GetItemClassData(FGameplayTag const ItemID) const;
	FItemDataRow* GetItemClassData(FName const ItemID) const;

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

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> ItemTable;

	std::atomic<bool> bIsInitialized{ false };
};
