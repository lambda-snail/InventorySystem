﻿#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveElement.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
 class UInteractiveElement : public UInterface
 {
 	GENERATED_BODY()
 };

/**
 * Represents an element that can receive interactions.
 */
class LAMBDASNAILINVENTORYSYSTEM_API IInteractiveElement
{
 GENERATED_BODY()

public:
 UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
 bool CanInteract() const;
 UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
 void Interact(AActor* Interactor);
};

