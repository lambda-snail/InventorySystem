#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveElement.generated.h"

UINTERFACE(MinimalAPI)
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
 FORCEINLINE virtual bool CanInteract() const = 0;
 virtual void Interact(AActor* Instigator) = 0;
};

