#include "Player/DesPlayerCharacter.h"

#include "DesGameplayTags.h"
#include "DesLogging.h"
#include "Actor/DesMetaComponent.h"
#include "Player/DesPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"
#include "Player/DesHeadBobComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/DesEnhancedInputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Player/DesPlayerAttributeSet.h"
#include "Player/Ability/DesGameplayAbilityPlayerInscribe.h"
#include "Player/DesPlayerAnimInstance.h"
#include "Player/Ability/Drag/DesGameplayAbilityPlayerGrab.h"

ADesPlayerCharacter::ADesPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UDesPlayerAttributeSet>(AttributeSetName))
{
	NetUpdateFrequency = 10.0f;

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	DefaultAbilities.Add(UDesGameplayAbilityPlayerGrab::StaticClass());

	MetaComponent->Name = FText::FromString(TEXT("PC"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetRootComponent());

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	HeadBob = CreateDefaultSubobject<UDesHeadBobComponent>(TEXT("HeadBob"));

	FirstPersonMeshSlot = CreateDefaultSubobject<USceneComponent>(TEXT("FirstPersonMeshSlot"));
	FirstPersonMeshSlot->SetupAttachment(Camera);

	const auto CharacterMesh = GetMesh();
	CharacterMesh->bEnablePerPolyCollision = false;
}

void ADesPlayerCharacter::InputLookAxisTriggered(const FInputActionInstance& Instance)
{
	LookAxis2D = Instance.GetValue().Get<FVector2D>();

	if (Cast<ADesPlayerController>(GetController())->IsLooking())
	{
		AddControllerYawInput(LookAxis2D.X);
		AddControllerPitchInput(LookAxis2D.Y);
	}
}

void ADesPlayerCharacter::InputMoveTriggered(const FInputActionInstance& Instance)
{
	const auto MoveValue = Instance.GetValue().Get<FVector2D>();
	AddMovementInput(Camera->GetForwardVector() * MoveValue.Y + Camera->GetRightVector() * MoveValue.X);
}

void ADesPlayerCharacter::InputAbilityPressed(const int32 InputID)
{
	AbilitySystemComponent->PressInputID(InputID);
}

void ADesPlayerCharacter::InputAbilityReleased(const int32 InputID)
{
	AbilitySystemComponent->ReleaseInputID(InputID);
}

void ADesPlayerCharacter::SetFirstPersonMode(const bool bFirstPerson) const
{
	if (const auto SkeletalMeshComponent = GetMesh())
	{
		if (bFirstPerson)
		{
			SkeletalMeshComponent->BoundsScale = 10.0f;
			SkeletalMeshComponent->bComponentUseFixedSkelBounds = true;
			SkeletalMeshComponent->SetSkeletalMeshAsset(FirstPersonMesh);
			SkeletalMeshComponent->AttachToComponent(FirstPersonMeshSlot,
			                                         FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
		else
		{
			SkeletalMeshComponent->BoundsScale = 1.0f;
			SkeletalMeshComponent->bComponentUseFixedSkelBounds = false;
			SkeletalMeshComponent->SetSkeletalMeshAsset(RegularMesh);
			SkeletalMeshComponent->AttachToComponent(RootComponent,
			                                         FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
		SkeletalMeshComponent->SetCastShadow(!bFirstPerson);
	}
}

void ADesPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetFirstPersonMode(IsLocallyControlled());

	const auto AnimInstance = Cast<UDesPlayerAnimInstance>(
		GetMesh()->GetAnimInstance());
	AnimInstance->InitializeForCharacter(this);
}

void ADesPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ADesPlayerCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void ADesPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto Velocity = GetVelocity().Length();
	CommonAudio->SetFloatParameter("WalkSpeed", Velocity);
	if (Velocity > 0.1 && !GetCharacterMovement()->IsFalling())
	{
		if (!CommonAudio->IsPlaying())
		{
			CommonAudio->Play();
		}
	}
	else
	{
		CommonAudio->Stop();
	}
}

void ADesPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const auto Input = Cast<UDesEnhancedInputComponent>(PlayerInputComponent);

	Input->BindActionByTag(InputConfig, TAG_Input_LookAxis, ETriggerEvent::Triggered, this,
	                       &ThisClass::InputLookAxisTriggered);
	Input->BindActionByTag(InputConfig, TAG_Input_Move, ETriggerEvent::Triggered, this,
	                       &ThisClass::InputMoveTriggered);

	Input->BindAbilities(InputConfig, this, &ThisClass::InputAbilityPressed, &ThisClass::InputAbilityReleased);
}

void ADesPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}