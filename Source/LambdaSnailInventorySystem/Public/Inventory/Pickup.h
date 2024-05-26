// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemBase.h"
#include "Pickup.generated.h"

class IInventoryActor;
class UWidgetComponent;
class AfpsCharacter;
class UItemType;
struct FItemInstance;
class USphereComponent;

/**
 * The Pickup is the physical representation of an inventory item in the world. It allows an item to be
 * picked up by a character.
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();

	void OnPickUp(AActor* Agent);
	
protected:
	virtual void BeginPlay() override;

	// TODO: We need an "item" interface maybe, that will have behavior for things such as pickup and throw etc
	UFUNCTION(BlueprintCallable)
	virtual void OnPickup(TScriptInterface<IInventoryActor> PickupCharacter);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	TObjectPtr<USphereComponent> Trigger;

	// If true, the player will not pick up the item immediately on overlap, but will instead be given an interaction
	// prompt asking if the item should be picked up.
	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool bInteractiveTrigger { true };

	// The collision channel to register to when the pickup has authority
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TEnumAsByte<ECollisionChannel> DefaultCollisionChannel { ECollisionChannel::ECC_Pawn };

	// The collision response to the default channel (only when the pickup has authority)
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TEnumAsByte<ECollisionResponse> DefaultCollisionResponse { ECollisionResponse::ECR_Overlap };
	
	// The class of item to spawn, if no item is assigned.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<UItemBase> ItemClass;

	// The item that will be picked up by this pickup. If Item is not null, then one
	// will be created using the ItemClass template.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UItemBase> Item;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
