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

TSoftObjectPtr<UItemDataAsset> UInventorySubsystem::GetItemClassData(FGameplayTag const ItemID) const
{
	return ItemDataMap.FindRef(ItemID);
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TArray<FAssetData> Assets;
	UAssetManager::Get().GetPrimaryAssetDataList(UItemDataAsset::StaticClass()->GetFName(), Assets);
	UE_LOGFMT(LogInventory, Display, "Found {Num} item data assets", Assets.Num());

	for (FAssetData const& Asset : Assets)
	{
		UItemDataAsset* ItemDataAsset = Cast<UItemDataAsset>(Asset.GetAsset());
		ItemDataMap.Add(ItemDataAsset->ID, TSoftObjectPtr<UItemDataAsset>(ItemDataAsset));
		ItemTags.AddLeafTag(ItemDataAsset->ID);
	}

	UE_LOGFMT(LogInventory, Display, "Loaded {Num} items", ItemDataMap.Num());

	bIsInitialized.store(true);
	OnSubsystemInitialized.Broadcast();

	UE_LOGFMT(LogInventory, Display, "Inventory subsystem {Name} loaded", this->GetName());

	// ULambdaSnail_InventorySettings const* Settings = GetDefault<ULambdaSnail_InventorySettings>();
	// checkf(Settings, TEXT("Failed to load inventory settings"));

	// FSoftObjectPath const& DataTablePath = Settings->InventoryTable;
	//
	// DataTablePath.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateLambda([this](FSoftObjectPath const& Path, UObject* Object) {
	// 	ItemTable = CastChecked<UDataTable>(Object);
	// 	UE_LOGFMT(LogInventory, Display, "Loaded data table: {Table}", Path.ToString());
	//
	// 	ItemTable->ForeachRow<FItemDataRow>(TEXT("Item Tags Initialization"), [this](FName const& Key, FItemDataRow const& ItemRow) {
	// 		ItemTags.AddLeafTag(ItemRow.ID);
	// 		UE_LOGFMT(LogInventory, Display, "Processed tag {Tag}", ItemRow.ID.ToString());
	// 	});
	//
	// 	UE_LOGFMT(LogInventory, Display, "Inventory subsystem initialized");
	// 	bIsInitialized.store(true);
	// 	OnSubsystemInitialized.Broadcast();
	// }));
}

void UInventorySubsystem::Deinitialize()
{
	Super::Deinitialize();
}