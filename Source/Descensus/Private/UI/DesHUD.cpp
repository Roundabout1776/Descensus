#include "UI/DesHUD.h"

#include "Actor/DesMetaComponent.h"
#include "UI/DesUIUtilities.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Player/DesInscriptionCanvas.h"
#include "Player/DesPlayerCharacter.h"
#include "Player/DesPlayerController.h"
#include "UI/DesGenericTooltip.h"
#include "UI/DesTooltipData.h"
#include "UI/DesWidget.h"
#include "UI/DesMainUILayer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

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
	FVector2D DesiredPosition;
	DesiredPosition.X = MousePosition.X;
	DesiredPosition.Y = TopLeftOut.Y;

	return DesiredPosition;
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
		GenericTooltip->UpdateTooltipPosition(MousePosition, true);
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
	GenericTooltip->UpdateTooltipPosition(GetDesiredTooltipPositionForActor(Actor), false);
}

void ADesHUD::BeginPlay()
{
	Super::BeginPlay();

	check(IsValid(GenericTooltipClass));
	GenericTooltip = CreateWidget<UDesGenericTooltip>(GetOwningPlayerController(), GenericTooltipClass);
	GenericTooltip->AddToPlayerScreen(1000);
}

void ADesHUD::ShowTooltip(FDesTooltipData TooltipData, FVector2D DesiredPosition, bool bShouldAddVerticalOffset)
{
	GenericTooltip->SetTooltipData(TooltipData);
	GenericTooltip->SetVisibility(ESlateVisibility::HitTestInvisible);
	GenericTooltip->UpdateTooltipPosition(DesiredPosition, bShouldAddVerticalOffset);
}

void ADesHUD::HideTooltip()
{
	GenericTooltip->SetVisibility(ESlateVisibility::Hidden);
	CurrentCursorTarget = ECursorTarget::None;
}

void ADesHUD::InitMainUILayer(const ADesPlayerCharacter* Character)
{
	const auto PlayerController = Cast<ADesPlayerController>(GetOwningPlayerController());
	
	check(IsValid(InscriptionCanvasClass));
	FActorSpawnParameters Parameters;
	Parameters.Owner = this;
	InscriptionCanvas = GetWorld()->SpawnActor<ADesInscriptionCanvas>(InscriptionCanvasClass, Parameters);
	InscriptionCanvas->DoInitialSetup(PlayerController->GetLocalPlayer()->ViewportClient->Viewport);
	
	check(IsValid(MainUILayerClass));
	MainUILayer = CreateWidget<UDesMainUILayer>(PlayerController, MainUILayerClass);
	MainUILayer->SetInscriptionOverlayMaterial(InscriptionCanvas->GetInscriptionCanvasMaterial());
	MainUILayer->AddToPlayerScreen(0);
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

