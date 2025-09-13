#include "Inventory/InventorySubsystem.h"

#include "Inventory/ItemDataRow.h"
#include "Logging/InventoryLogging.h"
#include "Settings/LambdaSnail_InventorySettings.h"

UInventorySubsystem* UInventorySubsystem::Get(UObject const* WorldContextObject)
{
	if (GEngine)
	{
		UWorld const* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UInventorySubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ULambdaSnail_InventorySettings const* Settings = GetDefault<ULambdaSnail_InventorySettings>();
	checkf(Settings, TEXT("Failed to load inventory settings"));

	FSoftObjectPath const& DataTablePath = Settings->InventoryTable;

	DataTablePath.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateLambda([this](FSoftObjectPath const& Path, UObject* Object) {
		ItemTable = CastChecked<UDataTable>(Object);
		UE_LOGFMT(InventorySystem, Display, "Loaded data table: {Table}", Path.ToString());

		ItemTable->ForeachRow<FItemDataRow>(TEXT("Item Tags Initialization"), [this](FName const& Key, FItemDataRow const& ItemRow) {
			ItemTags.AddLeafTag(ItemRow.Name);
			UE_LOGFMT(InventorySystem, Display, "Processed tag {Tag}", ItemRow.Name.ToString());
		});
	}));
}

void UInventorySubsystem::Deinitialize()
{
	Super::Deinitialize();
}