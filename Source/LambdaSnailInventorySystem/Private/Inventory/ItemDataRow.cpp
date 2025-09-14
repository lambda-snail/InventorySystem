#include "Inventory/ItemDataRow.h"

#include "DataTableEditorUtils.h"

void FItemDataRow::OnDataTableChanged(UDataTable const* InDataTable, FName const InRowName)
{
	FItemDataRow const* Row = InDataTable->FindRow<FItemDataRow>(InRowName, TEXT("OnDataTableChanged"));

	// Ugly but saves a ton of work since we don't need to subclass UDataTable to achieve this.
	// Since InDataTable isn't const originally, this shouldn't be UB. If the data table is accessed concurrently by the
	// editor, this could lead to a race condition, however. If this happens this needs to be replaced by a proper way of doing things.
	FDataTableEditorUtils::RenameRow(const_cast<UDataTable*>(InDataTable), InRowName, Row->ID.GetTagName());
}