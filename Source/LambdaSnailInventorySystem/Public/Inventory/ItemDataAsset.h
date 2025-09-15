// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 *
 */
UCLASS()
class LAMBDASNAILINVENTORYSYSTEM_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly, meta = (Categories = "Items"))
	FGameplayTag ID{};

	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly)
	FText DisplayName{};

	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly)
	FText Description{};

	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly)
	bool bCanStack{ false };

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LambdaSnail|Inventory", meta = (DisplayThumbnail = "true", AllowedClasses = "/Script/Engine.Texture2D,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	// UPROPERTY(EditAnywhere, Category = "LambdaSnail|Inventory", BlueprintReadOnly, meta = (MustImplement = "ItemInterface"))
	// TSubclassOf<class AActor> OverrideItemActor;
};
