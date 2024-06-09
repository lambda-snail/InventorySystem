// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Network/ReplicatedObject.h"
#include "ItemBase.generated.h"

class UItemType;

/**
 * The base class for all items with callbacks for inventory events. 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class LAMBDASNAILINVENTORYSYSTEM_API UItemBase : public UReplicatedObject
{
	GENERATED_BODY()
	
public:
	UItemBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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

	void ChangeOwner(AActor* NewOwner);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemType> ItemMetadata;
};
