// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "InventoryActionWidget.generated.h"

class UImage;
class UTextBlock;
class UInventoryAction;
/**
 * 
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API UInventoryActionWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionName;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ActionIcon;

	UPROPERTY(BlueprintReadOnly)
	UInventoryAction* Action;
};
