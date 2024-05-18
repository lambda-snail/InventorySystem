// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "EquipmentComponent.generated.h"

/**
 * 
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API UEquipmentComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	void ForEachEquipmentInstance(TFunction<void(UItemBase const&, FGameplayTag SlotTag, uint32 const Slot)> const& Callback) const;

	virtual EItemTransferResult TryMoveItem(FItemTransferRequest const& TransferRequest) override;
	
protected:
	virtual void BeginPlay() override;

	// TODO: Add way to map shortcuts to various slots
	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> EquipmentSlotTypes; 
};
