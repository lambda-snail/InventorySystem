#pragma once

#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "ItemDataRow.generated.h"

USTRUCT()
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (Categories = "Items"))
	FGameplayTag ID;

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FSlateBrush Icon;

	UPROPERTY(EditAnywhere, meta = (MustImplement = "ItemInterface"))
	TSubclassOf<class AActor> OverrideItemActor;

//~ Begin FTableRowBase Interface
#if WITH_EDITOR
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
#endif
	//~ End FTableRowBase Interface
};
