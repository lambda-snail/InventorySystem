#pragma once

#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "ItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly, meta = (Categories = "Items"))
	FGameplayTag ID{};

	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly)
	FText DisplayName{};

	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly)
	FText Description{};

	// UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly)
	// FSlateBrush Icon{};

	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly, meta = (MustImplement = "ItemInterface"))
	TSubclassOf<class AActor> OverrideItemActor;

//~ Begin FTableRowBase Interface
#if WITH_EDITOR
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
#endif
	//~ End FTableRowBase Interface
};
