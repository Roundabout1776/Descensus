#include "Player/DesPlayerController.h"

#include "AbilitySystemComponent.h"
#include "DesGameplayTags.h"
#include "UI/DesHUD.h"
#include "Actor/DesMetaComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Input/DesEnhancedInputComponent.h"
#include "Player/DesInscriptionCanvas.h"
#include "Player/DesPlayerCharacter.h"
#include "Slate/SceneViewport.h"

ADesPlayerController::ADesPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HitResultTraceDistance = 250;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;

	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;
}

void ADesPlayerController::AcknowledgePossession(APawn* InPawn)
{
	Super::AcknowledgePossession(InPawn);

	PlayerCharacter = MakeWeakObjectPtr(CastChecked<ADesPlayerCharacter>(InPawn));
	PlayerCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerCharacter.Get(), PlayerCharacter.Get());

	DesHUD = MakeWeakObjectPtr(GetHUD<ADesHUD>());
	DesHUD->InitMainUILayer(PlayerCharacter.Get());

	const auto LocalPlayer = GetLocalPlayer();
	const auto InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSystem->ClearAllMappings();
	if (InputMapping)
	{
		FModifyContextOptions Options;
		Options.bIgnoreAllPressedKeysUntilRelease = 1;
		Options.bForceImmediately = 1;
		InputSystem->AddMappingContext(InputMapping, 0, Options);
	}

	const auto Input = Cast<UDesEnhancedInputComponent>(InputComponent);
	Input->BindActionByTag(InputConfig, TAG_Input_Look, ETriggerEvent::Triggered, this,
	                       &ADesPlayerController::InputLookTriggered);
	Input->BindActionByTag(InputConfig, TAG_Input_Look, ETriggerEvent::Started, this,
	                       &ADesPlayerController::InputLookStarted);
	Input->BindActionByTag(InputConfig, TAG_Input_Look, ETriggerEvent::Completed, this,
	                       &ADesPlayerController::InputLookCompleted);
}

void ADesPlayerController::InputLookTriggered()
{
}

void ADesPlayerController::InputLookStarted()
{
	bIsLooking = true;

	const auto GameViewport = GEngine->GameViewport->GetGameViewport();
	GameViewport->GetMousePos(MousePosBeforeHiddenDueToCapture, true);

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	ResetIgnoreLookInput();

	ActorUnderCursor.Reset();

	DesHUD->LookStarted();
}

void ADesPlayerController::InputLookCompleted()
{
	bIsLooking = false;

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::LockAlways, false, false);
	SetIgnoreLookInput(true);

	const auto GameViewport = GEngine->GameViewport->GetGameViewport();
	GameViewport->SetMouse(MousePosBeforeHiddenDueToCapture.X, MousePosBeforeHiddenDueToCapture.Y);
	MousePosBeforeHiddenDueToCapture = FIntPoint(-1, -1);

	DesHUD->LookCompleted();
}

void ADesPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADesPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!DesHUD.IsValid())
	{
		return;
	}

	/* Handle stuff under cursor */
	if (!bIsLooking)
	{
		// const auto HUD = GetHUD<ADesHUD>();

		if (DesHUD->CurrentCursorTarget == ECursorTarget::Widget)
		{
			ActorUnderCursor.Reset();
			return;
		}

		if (const auto ComponentUnderCursor = CurrentClickablePrimitive.Get(); IsValid(ComponentUnderCursor))
		{
			const auto Actor = ComponentUnderCursor->GetOwner();

			if (ActorUnderCursor.Get() == Actor)
			{
				DesHUD->UpdateActorUnderCursor(Actor);
				return;
			}

			if (const auto MetaComponent = Actor->FindComponentByClass<UDesMetaComponent>(); IsValid(MetaComponent))
			{
				ActorUnderCursor = MakeWeakObjectPtr<AActor>(Actor);
				DesHUD->NewActorUnderCursor(Actor, MetaComponent);
				return;
			}
		}

		if (ActorUnderCursor.IsValid())
		{
			ActorUnderCursor.Reset();
			DesHUD->NewActorUnderCursor(nullptr, nullptr);
		}
	}
}

bool ADesPlayerController::ShouldShowMouseCursor() const
{
	return !bIsLooking;
}

void ADesPlayerController::GetMousePositionDetailed(FVector2D& Position, FVector2D& PositionNDC,
                                                    FVector2D& PositionNormalized)
{
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(this, Position.X, Position.Y);

	const auto Geometry = UWidgetLayoutLibrary::GetPlayerScreenWidgetGeometry(this);

	const auto ViewportSize = Geometry.Size;

	PositionNDC = Position / ViewportSize;
	PositionNDC *= 2.0;
	PositionNDC -= FVector2D::One();
	PositionNDC.Y *= -1.0;

	PositionNormalized = PositionNDC;
	PositionNormalized.Y /= ViewportSize.X / static_cast<double>(ViewportSize.Y);
}
