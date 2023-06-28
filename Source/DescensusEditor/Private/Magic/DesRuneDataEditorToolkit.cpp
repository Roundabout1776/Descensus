#include "Magic/DesRuneDataEditorToolkit.h"

#include "Widgets/Docking/SDockTab.h"
#include "Magic/DesRuneData.h"
#include "Magic/SDesRuneDataWidget.h"
#include "Modules/ModuleManager.h"

void FDesRuneDataEditorToolkit::InitEditor(const TArray<UObject*>& InObjects)
{
	RuneData = Cast<UDesRuneData>(InObjects[0]);
	GenerateTemplateIndices();

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("RuneDataEditorLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			                             ->Split
			                             (
				                             FTabManager::NewSplitter()
				                             ->SetSizeCoefficient(0.6f)
				                             ->SetOrientation(Orient_Horizontal)
				                             ->Split
				                             (
					                             FTabManager::NewStack()
					                             ->SetSizeCoefficient(0.8f)
					                             ->AddTab("RuneDataPlotTab", ETabState::OpenedTab)
				                             )
				                             ->Split
				                             (
					                             FTabManager::NewStack()
					                             ->SetSizeCoefficient(0.2f)
					                             ->AddTab("RuneDataDetailsTab", ETabState::OpenedTab)
				                             )
			                             )
		);
	InitAssetEditor(EToolkitMode::Standalone, {}, "RuneDataEditor", Layout, true, true,
	                InObjects);
}

void FDesRuneDataEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Rune Data Editor"));

	InTabManager->RegisterTabSpawner("RuneDataPlotTab", FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
	            {
		            return SNew(SDockTab)
		            [
			            SNew(SVerticalBox)
			            + SVerticalBox::Slot()
			            [
				            SNew(SBox)
				            [
					            SNew(SBorder)
								.Padding(32.0f)
								.BorderBackgroundColor(FSlateColor(FLinearColor::Red))
					            [
						            SAssignNew(PlotWidget, SDesRuneDataWidget)
									.PointsRaw(this, &FDesRuneDataEditorToolkit::GetPointsRaw)
									.Points(this, &FDesRuneDataEditorToolkit::GetPoints)
									.OnAddPointDelegate(this, &FDesRuneDataEditorToolkit::AddPointRaw)
					            ]
				            ]
			            ]
			            + SVerticalBox::Slot()
			            .AutoHeight()
			            [
				            SNew(SHorizontalBox)
				            + SHorizontalBox::Slot()
				              .AutoWidth()
				              .VAlign(VAlign_Center)
				            [
					            SAssignNew(TemplatesComboBox, SComboBox<TSharedPtr<int32>>)
					            .InitiallySelectedItem(SelectedTemplateIndex)
					            .OnComboBoxOpening_Lambda([this]()
					            {
						            GenerateTemplateIndices();
					            })
					            .OptionsSource(&TemplateIndices)
					            .OnSelectionChanged_Lambda(
						            [this](const TSharedPtr<int32> NewSelection,
						                   ESelectInfo::Type SelectInfo)
						            {
							            SelectedTemplateIndex = NewSelection;
						            })
					            .OnGenerateWidget_Lambda([](const TSharedPtr<int32> InIndex)
					            {
						            return SNew(STextBlock).Text(
							            FText::FromString(
								            FString::FromInt(*InIndex)));
					            })
					            [
						            SNew(STextBlock)
						            .Text_Lambda([this]()
						            {
							            return !SelectedTemplateIndex.IsValid()
								                   ? INVTEXT("Empty")
								                   : FText::FromString(FString::FromInt(*SelectedTemplateIndex));
						            })
					            ]
				            ]
				            + SHorizontalBox::Slot()
				              .AutoWidth()
				              .VAlign(VAlign_Center)
				            [
					            SNew(SButton)
					            .OnPressed(this, &FDesRuneDataEditorToolkit::OnAddTemplatePressedHandler)
					            .VAlign(VAlign_Center)
					            .HAlign(HAlign_Center)
					            [
						            SNew(STextBlock)
						            .Text(INVTEXT("Add Template"))
					            ]
				            ]
				            + SHorizontalBox::Slot()
				              .AutoWidth()
				              .VAlign(VAlign_Center)
				            [
					            SNew(SButton)
					            .OnPressed(this, &FDesRuneDataEditorToolkit::OnRemoveTemplatePressedHandler)
					            .VAlign(VAlign_Center)
					            .HAlign(HAlign_Center)
					            [
						            SNew(STextBlock)
						            .Text(INVTEXT("Remove Template"))
					            ]
				            ]
				            + SHorizontalBox::Slot()
				            [
					            SNew(SButton)
								.OnPressed(this, &FDesRuneDataEditorToolkit::OnResamplePressedHandler)
					            .VAlign(VAlign_Center)
					            .HAlign(HAlign_Center)
					            [
						            SNew(STextBlock)
						            .Text(INVTEXT("Resample"))
					            ]
				            ]
				            + SHorizontalBox::Slot()
				            [
					            SNew(SButton)
								.OnPressed(this, &FDesRuneDataEditorToolkit::OnResetPressedHandler)
					            .VAlign(VAlign_Center)
					            .HAlign(HAlign_Center)
					            [
						            SNew(STextBlock)
						            .Text(INVTEXT("Clear"))
					            ]
				            ]
			            ]
		            ];
	            }))
	            .SetDisplayName(INVTEXT("Plot"))
	            .SetGroup(WorkspaceMenuCategory.ToSharedRef());

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(
		"PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	const TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{RuneData});
	InTabManager->RegisterTabSpawner("RuneDataDetailsTab", FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
	            {
		            return SNew(SDockTab)
		            [
			            DetailsView
		            ];
	            }))
	            .SetDisplayName(INVTEXT("Details"))
	            .SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FDesRuneDataEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner("RuneDataPlotTab");
	InTabManager->UnregisterTabSpawner("RuneDataDetailsTab");
}

const TArray<FDesStrokePoint>* FDesRuneDataEditorToolkit::GetPointsRaw() const
{
	if (SelectedTemplateIndex == nullptr)
		return nullptr;
	if (const auto Index = *SelectedTemplateIndex.Get(); RuneData->Templates.IsValidIndex(Index))
	{
		return &RuneData->Templates[Index].PointsRaw;
	}
	return nullptr;
}

const TArray<FDesStrokePoint>* FDesRuneDataEditorToolkit::GetPoints() const
{
	if (SelectedTemplateIndex == nullptr)
		return nullptr;
	if (const auto Index = *SelectedTemplateIndex.Get(); RuneData->Templates.IsValidIndex(Index))
	{
		return &RuneData->Templates[Index].Points;
	}
	return nullptr;
}

void FDesRuneDataEditorToolkit::AddPointRaw(const FDesStrokePoint& Point) const
{
	if (SelectedTemplateIndex == nullptr)
		return;
	if (const auto Index = *SelectedTemplateIndex.Get(); RuneData->Templates.IsValidIndex(Index))
	{
		RuneData->Modify();
		RuneData->Templates[Index].PointsRaw.Add(Point);
	}
}

void FDesRuneDataEditorToolkit::OnAddTemplatePressedHandler()
{
	if (GEditor && GEditor->CanTransact() && ensure(!GIsTransacting))
	{
		GEditor->BeginTransaction(TEXT(""), INVTEXT("Add Rune Data Template"), nullptr);

		RuneData->Modify();
		RuneData->Templates.AddDefaulted();

		GEditor->EndTransaction();

		GenerateTemplateIndices();
	}
}

void FDesRuneDataEditorToolkit::OnRemoveTemplatePressedHandler()
{
	if (!SelectedTemplateIndex.IsValid()) return;

	const auto Index = *SelectedTemplateIndex;

	if (!RuneData->Templates.IsValidIndex(Index)) return;

	if (GEditor && GEditor->CanTransact() && ensure(!GIsTransacting))
	{
		GEditor->BeginTransaction(TEXT(""), INVTEXT("Remove Rune Data Template"), nullptr);

		RuneData->Modify();
		RuneData->Templates.RemoveAt(Index);

		GEditor->EndTransaction();

		GenerateTemplateIndices();
	}
}

void FDesRuneDataEditorToolkit::OnResetPressedHandler() const
{
	if (SelectedTemplateIndex == nullptr)
		return;
	if (const auto Index = *SelectedTemplateIndex.Get(); RuneData->Templates.IsValidIndex(Index))
	{
		if (GEditor && GEditor->CanTransact() && ensure(!GIsTransacting))
		{
			GEditor->BeginTransaction(TEXT(""), INVTEXT("Reset Rune Data"), nullptr);

			RuneData->Modify();
			RuneData->Templates[Index].PointsRaw.Empty();;
			RuneData->Templates[Index].Points.Empty();

			GEditor->EndTransaction();
		}
	}
}

void FDesRuneDataEditorToolkit::OnResamplePressedHandler() const
{
	if (SelectedTemplateIndex == nullptr)
		return;
	if (const auto Index = *SelectedTemplateIndex.Get(); RuneData->Templates.IsValidIndex(Index))
	{
		if (GEditor && GEditor->CanTransact() && ensure(!GIsTransacting))
		{
			GEditor->BeginTransaction(TEXT(""), INVTEXT("Reset Rune Data"), nullptr);

			RuneData->Modify();
			RuneData->Templates[Index].UpdateFromPointsRaw();

			GEditor->EndTransaction();
		}
	}
}

void FDesRuneDataEditorToolkit::GenerateTemplateIndices()
{
	if (!IsValid(RuneData)) return;

	int32 SelectedTemplateIndexOld = -1;
	if (SelectedTemplateIndex != nullptr)
	{
		SelectedTemplateIndexOld = *SelectedTemplateIndex.Get();
	}

	TemplateIndices.Empty();
	for (int Index = 0; Index < RuneData->Templates.Num(); ++Index)
	{
		TemplateIndices.Add(MakeShared<int>(Index));
	}

	if (TemplateIndices.IsValidIndex(SelectedTemplateIndexOld))
	{
		SelectedTemplateIndex = TemplateIndices[SelectedTemplateIndexOld];
	}
	else
	{
		SelectedTemplateIndex = TemplateIndices.Num() > 0 ? TemplateIndices.Last() : nullptr;
	}
	if (TemplatesComboBox.IsValid())
	{
		TemplatesComboBox->SetSelectedItem(SelectedTemplateIndex);
	}
}
