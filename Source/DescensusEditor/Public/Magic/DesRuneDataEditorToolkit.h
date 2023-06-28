#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

struct FDesStrokePoint;
class SDesRuneDataWidget;
class UDesRuneData;

class DESCENSUSEDITOR_API FDesRuneDataEditorToolkit : public FAssetEditorToolkit
{
	TSharedPtr<SComboBox<TSharedPtr<int32>>> TemplatesComboBox;
	TSharedPtr<int32> SelectedTemplateIndex;
	TArray<TSharedPtr<int32>> TemplateIndices;
	TSharedPtr<SDesRuneDataWidget> PlotWidget;
	UDesRuneData* RuneData = nullptr;

public:
	void InitEditor(const TArray<UObject*>& InObjects);

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual FName GetToolkitFName() const override { return "RuneDataEditor"; }
	virtual FText GetBaseToolkitName() const override { return INVTEXT("Rune Data Editor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return "Rune Data "; }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return {}; }

	const TArray<FDesStrokePoint>* GetPointsRaw() const;
	const TArray<FDesStrokePoint>* GetPoints() const;
	void AddPointRaw(const FDesStrokePoint& Point) const;
	void OnAddTemplatePressedHandler();
	void OnRemoveTemplatePressedHandler();
	void OnResetPressedHandler() const;
	void OnResamplePressedHandler() const;
	void GenerateTemplateIndices();
};
