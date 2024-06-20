#include "Inventory/Actions/InventoryAction.h"

void UInventoryAction::Invoke_Implementation()
{
}

bool UInventoryAction::IsValidInvocationTarget_Implementation() const
{
	return false;
}

void UInventoryAction::SetItem(UItemBase* RelatedItem)
{
	Item = RelatedItem;
}
