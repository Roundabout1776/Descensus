#include "DesEditorModule.h"

#include "AssetToolsModule.h"
#include "Magic/DesRuneDataActions.h"

IMPLEMENT_MODULE(FDesEditorModule, DescensusEditor)

#define LOCTEXT_NAMESPACE "FDescensusEditorModule"

void FDesEditorModule::StartupModule()
{
	FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(FName("Descensus"), INVTEXT("Descensus"));

	RuneDataActions = MakeShared<FDesRuneDataActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(RuneDataActions.ToSharedRef()->AsShared());
}

void FDesEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
