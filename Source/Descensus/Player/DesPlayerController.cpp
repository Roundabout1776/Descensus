#include "Player/DesPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DesGameplayTags.h"
#include "DesLogging.h"
#include "UI/DesHUD.h"
#include "Actor/DesMetaComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Input/DesEnhancedInputComponent.h"
#include "Player/DesInscriptionCanvas.h"
#include "Player/DesPlayerCharacter.h"
#include "Player/Ability/Drag/DesGameplayAbilityPlayerGrab.h"
#include "AbilitySystem/DesGameplayAbilityPrimaryBase.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"
#include "Actor/DesActor.h"
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
	                       &ThisClass::InputLookTriggered);
	Input->BindActionByTag(InputConfig, TAG_Input_Look, ETriggerEvent::Started, this,
	                       &ThisClass::InputLookStarted);
	Input->BindActionByTag(InputConfig, TAG_Input_Look, ETriggerEvent::Completed, this,
	                       &ThisClass::InputLookCompleted);
	Input->BindActionByTag(InputConfig, TAG_Input_Primary, ETriggerEvent::Started, this,
	                       &ThisClass::InputPrimaryStarted);
	Input->BindActionByTag(InputConfig, TAG_Input_Primary, ETriggerEvent::Completed, this,
	                       &ThisClass::InputPrimaryCompleted);
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

	MetaComponentUnderCursor.Reset();

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
		if (DesHUD->CurrentCursorTarget == ECursorTarget::Widget)
		{
			MetaComponentUnderCursor.Reset();
			return;
		}

		if (const auto ComponentUnderCursor = CurrentClickablePrimitive.Get(); IsValid(ComponentUnderCursor))
		{
			const auto ActorUnderCursor = ComponentUnderCursor->GetOwner();

			if (const auto MetaComponent = ActorUnderCursor->GetComponentByClass<UDesMetaComponent>(); IsValid(
				MetaComponent))
			{
				if (MetaComponentUnderCursor.Get() == MetaComponent)
				{
					DesHUD->UpdateActorUnderCursor(ActorUnderCursor);
					return;
				}

				MetaComponentUnderCursor = MakeWeakObjectPtr(MetaComponent);

				DesHUD->NewActorUnderCursor(ActorUnderCursor, MetaComponent);
				return;
			}
		}

		if (MetaComponentUnderCursor.IsValid())
		{
			MetaComponentUnderCursor.Reset();

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

void ADesPlayerController::InputPrimaryStarted()
{
	if (!PlayerCharacter.IsValid())
	{
		return;
	}
	auto ASC = Cast<UDesAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
	if (IsLooking())
	{
		ASC->PressAbilitiesByTag(FGameplayTagContainer(TAG_Ability_Primary), false);
	}
	else
	{
		ASC->PressAbilitiesByTag(FGameplayTagContainer(TAG_Ability_PlayerDrag), false);
		// if (ActorUnderCursor.IsValid())
		// {
		// 	if (const auto MetaComponent = ActorUnderCursor->FindComponentByClass<UDesMetaComponent>(); IsValid(MetaComponent))
		// 	{
		// 		DES_LOG(Warning, TEXT("Actor: %s"), *MetaComponent->Name.ToString())
		// 		auto Payload = FGameplayEventData();
		// 		Payload.Target = ActorUnderCursor.Get();
		// 		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter.Get(), TAG_Ability_PlayerDrag, Payload);
		// 	}
		// }
	}
}

void ADesPlayerController::InputPrimaryCompleted()
{
	if (!PlayerCharacter.IsValid())
	{
		return;
	}
	auto ASC = Cast<UDesAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
	if (IsLooking())
	{
		ASC->ReleaseAbilitiesByTag(FGameplayTagContainer(TAG_Ability_Primary), true);
	}
	else
	{
		ASC->ReleaseAbilitiesByTag(FGameplayTagContainer(TAG_Ability_PlayerDrag), true);
	}
}

UPrimitiveComponent* ADesPlayerController::GetCurrentClickablePrimitive(const bool bForGrab) const
{
	if (bForGrab && (!MetaComponentUnderCursor.IsValid() || !MetaComponentUnderCursor->CanBeGrabbed()))
	{
		return nullptr;
	}
	return CurrentClickablePrimitive.Get();
}
