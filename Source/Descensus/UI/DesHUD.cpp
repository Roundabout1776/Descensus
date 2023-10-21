#include "DesHUD.h"

#include "Components/DesMetaComponent.h"
#include "UI/DesUIUtilities.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Player/DesInscriptionCanvas.h"
#include "Player/DesPlayerCharacter.h"
#include "Player/DesPlayerController.h"
#include "DesTooltipData.h"
#include "DesMainUILayer.h"
#include "SDesHUDLayer.h"
#include "Components/DesItemContainerComponent.h"
#include "Components/Image.h"
#include "Items/DesItemData.h"
#include "SDesItemWidget.h"
#include "SDesPopupLayer.h"
#include "Player/DesInventoryComponent.h"
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

ADesHUD::ADesHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADesHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(
		GetOwningPlayerController());

	if (PopupLayer.IsValid())
	{
		PopupLayer->SetEjectedItemPosition(MousePosition);
	}

	if (SlateUser.IsValid())
	{
		IDesTooltip* Tooltip{};
		bool bNewTooltipWidget{};
		for (auto& WidgetWeak : SlateUser->GetLastWidgetsUnderCursor().Widgets)
		{
			const auto WidgetPinned = WidgetWeak.Pin();
			if (WidgetPinned.IsValid())
			{
				const auto MetaData = WidgetPinned->GetMetaData<FDesTooltipMetaData>();
				if (MetaData.IsValid())
				{
					if (!MetaData->Tooltip->ShouldShowTooltip())
					{
						continue;
					}

					if (!LastTooltipWidgetUnderCursor.HasSameObject(WidgetPinned.Get()))
					{
						bNewTooltipWidget = true;
						LastTooltipWidgetUnderCursor = WidgetWeak;
					}

					Tooltip = MetaData->Tooltip;
					break;
				}
			}
		}

		if (Tooltip)
		{
			if (bNewTooltipWidget || Tooltip->IsTooltipDirty())
			{
				ShowTooltip(Tooltip->GetCachedTooltipData(), MousePosition, true);
			}
			else
			{
				PopupLayer->SetTooltipPosition(MousePosition, true);
			}

			CurrentCursorTarget = ECursorTarget::Widget;
		}
		else
		{
			LastTooltipWidgetUnderCursor.Reset();
			if (CurrentCursorTarget == ECursorTarget::Widget)
			{
				HideTooltip();
			}
		}
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
	PopupLayer->SetTooltipPosition(GetDesiredTooltipPositionForActor(Actor), false);
}

void ADesHUD::ShowTooltip(const FDesTooltipData& TooltipData, const FVector2D DesiredPosition,
                          const bool bShouldAddVerticalOffset) const
{
	PopupLayer->SetTooltipData(TooltipData);
	PopupLayer->SetTooltipPosition(DesiredPosition, bShouldAddVerticalOffset);
	PopupLayer->SetTooltipVisible(true);
}

void ADesHUD::HideTooltip()
{
	PopupLayer->SetTooltipVisible(false);
	CurrentCursorTarget = ECursorTarget::None;
}

void ADesHUD::InitForCharacter(const ADesPlayerCharacter* Character)
{
	InventoryComponent = MakeWeakObjectPtr(Character->Inventory);

	const auto PlayerController = Cast<ADesPlayerController>(GetOwningPlayerController());
	const auto LocalPlayer = PlayerController->GetLocalPlayer();
	const auto ViewportClient = LocalPlayer->ViewportClient;

	check(IsValid(InscriptionCanvasClass));
	FActorSpawnParameters Parameters;
	Parameters.Owner = this;
	InscriptionCanvas = GetWorld()->SpawnActor<ADesInscriptionCanvas>(InscriptionCanvasClass, Parameters);
	InscriptionCanvas->DoInitialSetup(ViewportClient->Viewport);

	SlateUser = LocalPlayer->GetSlateUser();

	PopupLayer = SNew(SDesPopupLayer);
	ViewportClient->AddViewportWidgetContent(PopupLayer.ToSharedRef(), PopupLayerZ);

	HUDLayer = SNew(SDesHUDLayer);
	HUDLayer->SetupItemSystem(InventoryComponent.Get());
	ViewportClient->AddViewportWidgetContent(HUDLayer.ToSharedRef(), HUDLayerZ);

	check(IsValid(MainUILayerClass));
	MainUILayer = CreateWidget<UDesMainUILayer>(PlayerController, MainUILayerClass);
	MainUILayer->AddToPlayerScreen(MainLayerZ);
	MainUILayer->InscriptionOverlay->SetBrushFromMaterial(InscriptionCanvas->GetInscriptionCanvasMaterial());

	InventoryComponent->OnEjectedItemChanged.AddUObject(this, &ThisClass::OnEjectedItemChanged);
	InventoryComponent->OnAnyChangesDelegate.AddUObject(this, &ThisClass::OnAnyInventoryChanges);
}

void ADesHUD::LookStarted()
{
	HideTooltip();
	HUDLayer->SetCrosshairVisible(true);
}

void ADesHUD::LookCompleted()
{
	HUDLayer->SetCrosshairVisible(false);
}

void ADesHUD::OnEjectedItemChanged(const UDesItemInstance* EjectedItem) const
{
	if (!PopupLayer.IsValid())
	{
		return;
	}

	if (EjectedItem)
	{
		PopupLayer->ShowEjectedItem(EjectedItem);
		SlateUser->SetCursorVisibility(false);
	}
	else
	{
		PopupLayer->HideEjectedItem();
		SlateUser->SetCursorVisibility(true);
	}
}

void ADesHUD::OnAnyInventoryChanges(const TArray<FItemContainerEntry>& ItemContainerEntries) const
{
	if (!PopupLayer.IsValid())
	{
		return;
	}

	if (const auto EjectedItem = InventoryComponent->GetEjectedItem())
	{
		PopupLayer->UpdateEjectedItemQuantity(EjectedItem->GetQuantity(), EjectedItem->GetItemData()->MaxQuantity);
	}
}
