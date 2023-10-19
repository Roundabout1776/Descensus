#include "UI/DesHUD.h"

#include "DesCharacterScreen.h"
#include "Components/DesMetaComponent.h"
#include "UI/DesUIUtilities.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Player/DesInscriptionCanvas.h"
#include "Player/DesPlayerCharacter.h"
#include "Player/DesPlayerController.h"
#include "DesTooltipData.h"
#include "DesWidget.h"
#include "DesMainUILayer.h"
#include "SDesTooltipLayer.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Components/DesItemContainerComponent.h"
#include "Components/Image.h"
#include "Items/DesItemContainerWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWeakWidget.h"

FVector2D ADesHUD::GetDesiredTooltipPositionForActor(const AActor* Actor) const
{
	const auto PlayerController = GetOwningPlayerController();
	FVector2D TopLeftOut;
	FVector2D BottomRightOut;
	UDesUIUtilities::GetActorBoundsOnScreen(PlayerController, Actor,
	                                        TopLeftOut, BottomRightOut);

	FVector2D MousePosition;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, MousePosition.X,
	                                                  MousePosition.Y);
	// MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(PlayerController);
	// PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	FVector2D DesiredPosition;
	DesiredPosition.X = MousePosition.X;
	DesiredPosition.Y = TopLeftOut.Y;

	return DesiredPosition;
}

void ADesHUD::CreateSlateWidgetAndAddToViewport(const TSharedRef<SWidget>& Widget, const int32 ZOrder) const
{
	const auto LocalPlayer = GetOwningPlayerController()->GetLocalPlayer();
	LocalPlayer->ViewportClient->AddViewportWidgetContent(Widget, ZOrder);
}

void ADesHUD::NewWidgetUnderCursor(UDesWidget* Widget)
{
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(
		GetOwningPlayerController());

	ShowTooltip(Widget->GetTooltipData(true), MousePosition, true);

	CurrentCursorTarget = ECursorTarget::Widget;
}

void ADesHUD::UpdateWidgetUnderCursor(UDesWidget* Widget)
{
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(
		GetOwningPlayerController());

	if (Widget->IsTooltipDirty())
	{
		ShowTooltip(Widget->GetTooltipData(true), MousePosition, true);
	}
	else
	{
		TooltipLayer->SetTooltipPosition(MousePosition, true);
	}
}

void ADesHUD::NewActorUnderCursor(const AActor* Actor, const UDesMetaComponent* MetaComponent)
{
	if (IsValid(MetaComponent))
	{
		ShowTooltip(MetaComponent->GetTooltipData(), GetDesiredTooltipPositionForActor(Actor), false);
		CurrentCursorTarget = ECursorTarget::Actor;
	}
	else
	{
		HideTooltip();
	}
}

void ADesHUD::UpdateActorUnderCursor(const AActor* Actor) const
{
	// const auto ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this) /
	// 	UWidgetLayoutLibrary::GetViewportScale(this);
	TooltipLayer->SetTooltipPosition(GetDesiredTooltipPositionForActor(Actor), false);
}

void ADesHUD::ShowTooltip(FDesTooltipData TooltipData, FVector2D DesiredPosition, bool bShouldAddVerticalOffset)
{
	TooltipLayer->SetTooltipData(TooltipData);
	TooltipLayer->SetVisibility(EVisibility::HitTestInvisible);
	TooltipLayer->SetTooltipPosition(DesiredPosition, bShouldAddVerticalOffset);
}

void ADesHUD::HideTooltip()
{
	TooltipLayer->SetVisibility(EVisibility::Hidden);
	CurrentCursorTarget = ECursorTarget::None;
}

void ADesHUD::InitForCharacter(const ADesPlayerCharacter* Character)
{
	const auto PlayerController = Cast<ADesPlayerController>(GetOwningPlayerController());

	check(IsValid(InscriptionCanvasClass));
	FActorSpawnParameters Parameters;
	Parameters.Owner = this;
	InscriptionCanvas = GetWorld()->SpawnActor<ADesInscriptionCanvas>(InscriptionCanvasClass, Parameters);
	InscriptionCanvas->DoInitialSetup(PlayerController->GetLocalPlayer()->ViewportClient->Viewport);

	check(IsValid(MainUILayerClass));
	MainUILayer = CreateWidget<UDesMainUILayer>(PlayerController, MainUILayerClass);
	MainUILayer->AddToPlayerScreen(MainLayerZ);
	MainUILayer->InscriptionOverlay->SetBrushFromMaterial(InscriptionCanvas->GetInscriptionCanvasMaterial());
	MainUILayer->SetupItemSystem(Character->Inventory);

	CreateSlateWidgetAndAddToViewport(SAssignNew(TooltipLayer, SDesTooltipLayer), TooltipLayerZ);
}

void ADesHUD::LookStarted()
{
	HideTooltip();
	MainUILayer->SetCrosshairVisible(true);
}

void ADesHUD::LookCompleted()
{
	MainUILayer->SetCrosshairVisible(false);
}
