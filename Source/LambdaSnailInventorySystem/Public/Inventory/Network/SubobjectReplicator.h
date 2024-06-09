#pragma once

#include "CoreMinimal.h"
#include "SubobjectReplicator.generated.h"

class UReplicatedObject;

UINTERFACE(MinimalAPI)
class USubobjectReplicator : public UInterface
{
	GENERATED_BODY()
};

/**
 * Allows subobjects to register their own subobjects for replication with a parent. This allows a parent actor or component
 * to be responsible for replicating more complicated structures, without having full knowledge of ho that structure works
 * or without the need for detailed bookkeeping logic in the wrong place.
 */
class LAMBDASNAILINVENTORYSYSTEM_API ISubobjectReplicator
{
	GENERATED_BODY()

public:
	virtual void AddReplicatedObject(UReplicatedObject* Object) = 0;
	virtual void RemoveReplicatedObject(UReplicatedObject* Object) = 0;
};