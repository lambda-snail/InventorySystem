#include "Inventory/InventorySubsystem.h"

#include "Engine/AssetManager.h"
#include "Inventory/ItemDataAsset.h"
#include "Inventory/ItemDataRow.h"
#include "Inventory/Logging/InventoryLogging.h"
#include "Inventory/Settings/LambdaSnail_InventorySettings.h"

UInventorySubsystem* UInventorySubsystem::Get(UObject const* WorldContextObject)
{
	if (GEngine)
	{
		UWorld const* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UInventorySubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UInventorySubsystem::GetItemClassData(FName ItemID, FItemDataRow& OutDataRow, bool& Success) const
{
	FItemDataRow* DataRow = GetItemClassData(ItemID);
	if (DataRow)
	{
		OutDataRow = *DataRow;
		Success = true;
		return;
	}

	Success = false;
}

FItemDataRow* UInventorySubsystem::GetItemClassData(FName const ItemID) const
{
	if (ItemTable)
	{
		return ItemTable->FindRow<FItemDataRow>(ItemID, "UInventorySubsystem");
	}

	return nullptr;
}

FItemDataRow* UInventorySubsystem::GetItemClassData(FGameplayTag const ItemID) const
{
	return ItemTable->FindRow<FItemDataRow>(ItemID.GetTagName(), "UInventorySubsystem");
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	auto XX = UItemDataAsset::StaticClass()->GetFName();

	auto* Y = NewObject<UItemDataAsset>();
	auto  Z = Y->GetPrimaryAssetId();

	TArray<FAssetData> Assets;
	UAssetManager::Get().GetPrimaryAssetDataList(UItemDataAsset::StaticClass()->GetFName(), Assets);
	UE_LOGFMT(LogInventory, Display, "Found {Num} item data assets", Assets.Num());

	for (FAssetData const& Asset : Assets)
	{
		UItemDataAsset* ItemDataAsset = Cast<UItemDataAsset>(Asset.GetAsset());
		ItemDataMap.Add(ItemDataAsset->ID, TSoftObjectPtr<UItemDataAsset>(ItemDataAsset));
		ItemTags.AddLeafTag(ItemDataAsset->ID);
		UE_LOGFMT(LogInventory, Display, "Processed item {Tag}", ItemDataAsset->ID.ToString());
	}

	// UE_LOGFMT(LogInventory, Display, "Inventory subsystem initialized");
	// bIsInitialized.store(true);
	// OnSubsystemInitialized.Broadcast();

	// UAssetManager::Get().LoadPrimaryAssets
	// //UAssetManager::Get().GetStreamableManager().RequestAsyncLoad();
	// //UAssetManager::Get().GetStreamableManager().

	ULambdaSnail_InventorySettings const* Settings = GetDefault<ULambdaSnail_InventorySettings>();
	checkf(Settings, TEXT("Failed to load inventory settings"));

	FSoftObjectPath const& DataTablePath = Settings->InventoryTable;

	DataTablePath.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateLambda([this](FSoftObjectPath const& Path, UObject* Object) {
		ItemTable = CastChecked<UDataTable>(Object);
		UE_LOGFMT(LogInventory, Display, "Loaded data table: {Table}", Path.ToString());

		ItemTable->ForeachRow<FItemDataRow>(TEXT("Item Tags Initialization"), [this](FName const& Key, FItemDataRow const& ItemRow) {
			ItemTags.AddLeafTag(ItemRow.ID);
			UE_LOGFMT(LogInventory, Display, "Processed tag {Tag}", ItemRow.ID.ToString());
		});

		UE_LOGFMT(LogInventory, Display, "Inventory subsystem initialized");
		bIsInitialized.store(true);
		OnSubsystemInitialized.Broadcast();
	}));
}

void UInventorySubsystem::Deinitialize()
{
	Super::Deinitialize();
}