#include "Magic/DesRuneDataActions.h"

#include "Magic/DesRuneData.h"
#include "Magic/DesRuneDataEditorToolkit.h"

void FDesRuneDataActions::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                          TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	MakeShared<FDesRuneDataEditorToolkit>()->InitEditor(InObjects);
}

FText FDesRuneDataActions::GetName() const
{
	return INVTEXT("Rune Data");
}

UClass* FDesRuneDataActions::GetSupportedClass() const
{
	return UDesRuneData::StaticClass();
}

FColor FDesRuneDataActions::GetTypeColor() const
{
	return FColor(128, 32, 230);
}

uint32 FDesRuneDataActions::GetCategories()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		return FAssetToolsModule::GetModule().Get().FindAdvancedAssetCategory(FName("Descensus"));
	}
	return EAssetTypeCategories::Misc;
}
