// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

class UItemWidget;
/**
 * 
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UItemWidget* GetWidgetReference() const;
	UFUNCTION(BlueprintCallable)
	void SetWidgetReference(UItemWidget* Widget);

	UFUNCTION(BlueprintCallable)
	FVector2D GetDragOffset() const;
	UFUNCTION(BlueprintCallable)
	void SetDragOffset(FVector2D NewDragOffset);
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemWidget> WidgetReference;

	UPROPERTY(BlueprintReadWrite)
	FVector2D DragOffset;
};
