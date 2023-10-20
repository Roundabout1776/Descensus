#include "Player/DesPlayerController.h"

#include "AbilitySystemComponent.h"
#include "DesGameplayTags.h"
#include "UI/DesHUD.h"
#include "Components//DesMetaComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Ability/Grab/DesGameplayAbilityPlayerGrab.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Input/DesEnhancedInputComponent.h"
#include "Player/DesPlayerCharacter.h"
#include "Character/Ability/DesGameplayAbilityPrimaryBase.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"
#include "Actors/DesContainerActor.h"
#include "Net/UnrealNetwork.h"
#include "Slate/SceneViewport.h"
#include "UI/DesMainUILayer.h"
#include "UI/SDesHUDLayer.h"

ADesPlayerController::ADesPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicateUsingRegisteredSubObjectList = true;
	HitResultTraceDistance = 250;
	bShowMouseCursor = true;

	bEnableMouseOverEvents = false;
	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;
}

void ADesPlayerController::AcknowledgePossession(APawn* InPawn)
{
	Super::AcknowledgePossession(InPawn);

	PlayerCharacter = MakeWeakObjectPtr(CastChecked<ADesPlayerCharacter>(InPawn));
	PlayerCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerCharacter.Get(), PlayerCharacter.Get());

	DesHUD = MakeWeakObjectPtr(GetHUD<ADesHUD>());
	DesHUD->InitForCharacter(PlayerCharacter.Get());

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

	if (!IsLocalController())
		return;

	if (!DesHUD.IsValid())
	{
		return;
	}

	/* Handle actor under cursor. */
	if (!bIsLooking)
	{
		FVector2D MousePosition;
		FHitResult HitResult;
		bool bHit = false;

		if (const UGameViewportClient* ViewportClient = Cast<ULocalPlayer>(Player)->ViewportClient; ViewportClient->
			GetMousePosition(MousePosition))
		{
			bHit = GetHitResultAtScreenPosition(MousePosition, CurrentClickTraceChannel, true, HitResult);
		}

		CurrentClickablePrimitive = bHit ? HitResult.Component.Get() : nullptr;

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
	const auto ASC = PlayerCharacter->CustomASC.Get();
	if (IsLooking())
	{
		ASC->PressAbilitiesByTag(FGameplayTagContainer(TAG_Ability_Primary), false);
	}
	else
	{
		ASC->PressAbilitiesByTag(FGameplayTagContainer(TAG_Ability_PlayerGrab), false);
	}
}

void ADesPlayerController::InputPrimaryCompleted()
{
	if (!PlayerCharacter.IsValid())
	{
		return;
	}
	const auto ASC = PlayerCharacter->CustomASC.Get();
	if (IsLooking())
	{
		ASC->ReleaseAbilitiesByTag(FGameplayTagContainer(TAG_Ability_Primary), true);
	}
	else
	{
		if (CurrentClickablePrimitive.IsValid())
		{
			/* Attempt to interact with a container. */
			if (const auto ContainerActor = Cast<ADesContainerActor>(CurrentClickablePrimitive->GetOwner()))
			{
				ServerOpenContainer(ContainerActor);
			}
			else
			{
				/* Otherwise, attempt to grab. */
				ASC->ReleaseAbilitiesByTag(FGameplayTagContainer(TAG_Ability_PlayerGrab), true);
			}
		}
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

bool ADesPlayerController::CheckIfCanInteractWithActor(const AActor* Actor)
{
	const auto Length = (Actor->GetActorLocation() - GetPawn()->GetActorLocation()).Length();
	return Length < HitResultTraceDistance;
}

void ADesPlayerController::OnRep_CurrentContainer() const
{
	if (IsLocalController())
	{
		DesHUD->GetHUDLayer()->SetCurrentContainer(CurrentContainer);
	}
}

void ADesPlayerController::ServerOpenContainer_Implementation(ADesContainerActor* ContainerActor)
{
	if (!ContainerActor)
	{
		return;
	}

	if (!CheckIfCanInteractWithActor(ContainerActor))
	{
		return;
	}

	ContainerActor->SetOpened();

	CurrentContainer = ContainerActor->ItemContainer;
	OnRep_CurrentContainer();
}

void ADesPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADesPlayerController, CurrentContainer);
}
