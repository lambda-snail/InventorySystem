#pragma once

#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "ItemDataRow.generated.h"

USTRUCT()
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag Name;

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FSlateBrush Icon;

	UPROPERTY(EditAnywhere, meta = (MustImplement = "IItemInterface"))
	TSubclassOf<class AActor> OverrideItemActor;

	//~ Begin FTableRowBase Interface
	// #if WITH_EDITOR
	// 	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// #endif
	//~ End FTableRowBase Interface
};
