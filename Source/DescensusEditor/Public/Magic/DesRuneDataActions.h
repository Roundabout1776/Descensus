#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class DESCENSUSEDITOR_API FDesRuneDataActions : public FAssetTypeActions_Base
{
public:
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
	                             TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
};
