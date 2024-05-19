// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Inventory/ItemBase.h"
#include "InertItem.generated.h"

class AStaticMeshActor;
/**
 * A simple item type that can be placed in the inventory but has no related logic.
 */
UCLASS(Blueprintable, BlueprintType)
class LAMBDASNAILINVENTORYSYSTEM_API UInertItem : public UItemBase
{
	GENERATED_BODY()

public:
	virtual void OnPickup_Implementation(AActor* Owner) override;
	virtual void OnDrop_Implementation(AActor* Owner) override;

	virtual void CreateComponentForPickup(AActor* Actor) const override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<UStaticMeshComponent> PhysicalRepresentationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;
};
