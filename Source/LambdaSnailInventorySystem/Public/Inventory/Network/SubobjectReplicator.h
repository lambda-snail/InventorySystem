#pragma once

#include "CoreMinimal.h"
#include "SubobjectReplicator.generated.h"

class UReplicatedObject;

UINTERFACE(MinimalAPI)
class USubobjectReplicator : public UInterface
{
	GENERATED_BODY()
};

class LAMBDASNAILINVENTORYSYSTEM_API ISubobjectReplicator
{
	GENERATED_BODY()

public:
	virtual void AddReplicatedObject(UReplicatedObject* Object) = 0;
	virtual void RemoveReplicatedObject(UReplicatedObject* Object) = 0;
};