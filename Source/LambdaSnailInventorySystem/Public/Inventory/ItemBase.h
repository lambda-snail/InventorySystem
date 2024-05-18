// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.generated.h"

class UItemType;

/**
 * The base class for all items with callbacks for inventory events. 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class LAMBDASNAILINVENTORYSYSTEM_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemBase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UItemType* GetItemData() const;

	// Called when an item is added to the inventory
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPickup(AActor* Owner);

	// Called when an item is dropped from the inventory
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDrop(AActor* Owner);

	/**
	 * Returns true if the other item is of the same type as this. This comparison is based on the
	 * asset used to define the inventory item. Used to determine if items can be stacked.
	 */
	bool IsSameItem(UItemBase const& Other) const;

	/**
	 * Allows the item to create its own visual representation when being spawned as part of a pickup
	 * @param Actor 
	 */
	virtual void CreateComponentForPickup(AActor* Actor) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemType> ItemMetadata;
};
