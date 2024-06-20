// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryAction.generated.h"

class UItemBase;
class UInventoryComponent;

/**
 * The base class for all inventory actions - i.e., actions that can be enumerated and executed
 * on certain items in the inventory.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush Icon;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Invoke();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsValidInvocationTarget() const;

	void SetItem(UItemBase* RelatedItem);
protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UItemBase> Item;
};
