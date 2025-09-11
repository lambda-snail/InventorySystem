#include "Inventory/InventorySubsystem.h"

UInventorySubsystem* UInventorySubsystem::Get(UObject const* WorldContextObject)
{
	if (GEngine)
	{
		UWorld const* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UInventorySubsystem>(World->GetGameInstance());
	}

	return nullptr;
}