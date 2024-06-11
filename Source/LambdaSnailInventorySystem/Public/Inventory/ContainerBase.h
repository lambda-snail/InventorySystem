#pragma once

#include "CoreMinimal.h"
#include "InteractiveElement.h"
#include "InventoryActor.h"
#include "GameFramework/Actor.h"
#include "ContainerBase.generated.h"

class UItemBase;
class UInventoryComponent;

UCLASS(Abstract, Blueprintable, BlueprintType)
class LAMBDASNAILINVENTORYSYSTEM_API AContainerBase : public AActor, public IInteractiveElement, public IInventoryActor
{
	GENERATED_BODY()

public:
	AContainerBase();
	
	virtual bool CanInteract_Implementation() const override;;
	virtual void Interact_Implementation(AActor* Interactor) override;;
	
	UFUNCTION(BlueprintCallable)
	virtual UInventoryComponent* GetInventoryComponent() const override { return InventoryComponent; }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
