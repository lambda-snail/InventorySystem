#pragma once
#include "GameplayTagContainer.h"

#include "ItemDetails.generated.h"

USTRUCT(BlueprintType)
struct FItemDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LambdaSnail|Inventory")
	FGameplayTag ItemClassId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LambdaSnail|Inventory")
	int32 ItemInstanceId{ 0 };

	bool FORCEINLINE IsStatic() const { return ItemInstanceId == 0; }
	bool FORCEINLINE HasInstance() const { return not IsStatic(); }
};