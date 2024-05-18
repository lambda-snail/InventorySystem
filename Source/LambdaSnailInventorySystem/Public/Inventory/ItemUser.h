#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemUser.generated.h"

UINTERFACE(MinimalAPI)
class UItemUser : public UInterface
{
	GENERATED_BODY()
};

class LAMBDASNAILINVENTORYSYSTEM_API IItemUser
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual void EnterTriggerArea(AActor* SourceActor) = 0;
	FORCEINLINE virtual void LeaveTriggerArea() = 0;
};
