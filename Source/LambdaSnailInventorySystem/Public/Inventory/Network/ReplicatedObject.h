// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReplicatedObject.generated.h"

/**
 * 
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API UReplicatedObject : public UObject
{
	GENERATED_BODY()

public:
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
	
	
	FORCEINLINE virtual bool IsSupportedForNetworking() const override { return true; };
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetOwner() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void DestroyObject();
};
