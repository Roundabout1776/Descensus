#pragma once

#include "CoreMinimal.h"
#include "DesWidgetStyleCatalog.generated.h"

USTRUCT()
struct DESCENSUS_API FDesSlateWidgetStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float CellSize = 50.0f;

	UPROPERTY(EditDefaultsOnly)
	float Padding = 19.0f;

	UPROPERTY(EditDefaultsOnly)
	FTextBlockStyle DefaultTextStyle;

	UPROPERTY(EditDefaultsOnly)
	FSlateBrush CommonBox;

	UPROPERTY(EditDefaultsOnly)
	FSlateBrush TestBrush;

	UPROPERTY(EditDefaultsOnly)
	FSlateBrush ItemContainerTelegraphBrush;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor ItemGridColor = FLinearColor::White;

	static const FName TypeName;

	virtual const FName GetTypeName() const override
	{
		return TypeName;
	}

	static const FDesSlateWidgetStyle& GetDefault()
	{
		static FDesSlateWidgetStyle Default;
		return Default;
	}

	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override
	{
		OutBrushes.Add(&CommonBox);
		OutBrushes.Add(&TestBrush);
		OutBrushes.Add(&ItemContainerTelegraphBrush);
	}
};

UCLASS()
class DESCENSUS_API UDesWidgetStyleCatalog : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ShowOnlyInnerProperties))
	FDesSlateWidgetStyle WidgetStyle;

public:
	virtual const FSlateWidgetStyle* const GetStyle() const override
	{
		return &WidgetStyle;
	}
};
