#include "Player/DesPlayerCharacter.h"

#include "DesGameplayTags.h"
#include "DesPlayerState.h"
#include "Actor/DesMetaComponent.h"
#include "Player/DesPlayerController.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"
#include "Player/DesHeadBobComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/DesEnhancedInputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Player/DesPlayerAttributeSet.h"
#include "Player/Ability/Inscribe/DesGameplayAbilityPlayerInscribe.h"
#include "Player/DesPlayerAnimInstance.h"

ADesPlayerCharacter::ADesPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UDesPlayerAttributeSet>(UDesCharacterAttributeSet::AttributeSetName))
{
	NetUpdateFrequency = 100.0f;

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
	GetAbilitySystemComponent()->PressInputID(InputID);
}

void ADesPlayerCharacter::InputAbilityReleased(const int32 InputID)
{
	GetAbilitySystemComponent()->ReleaseInputID(InputID);
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
}

void ADesPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (const auto PS = GetPlayerState<ADesPlayerState>())
	{
		ASCWeakPtr = MakeWeakObjectPtr(PS->ASC);

		ASCWeakPtr->InitAbilityActorInfo(PS, this);

		GiveDefaultAbilities();
		ApplyDefaultEffects();

		SetOwner(NewController);
	}
}

void ADesPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (const auto PS = GetPlayerState<ADesPlayerState>())
	{
		ASCWeakPtr = MakeWeakObjectPtr(PS->ASC);

		ASCWeakPtr->InitAbilityActorInfo(PS, this);

		const auto AnimInstance = Cast<UDesPlayerAnimInstance>(
			GetMesh()->GetAnimInstance());
		AnimInstance->InitializeForCharacter(this);
	}
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

UAbilitySystemComponent* ADesPlayerCharacter::GetAbilitySystemComponent() const
{
	return GetPlayerStateChecked<ADesPlayerState>()->GetAbilitySystemComponent();
}

UAttributeSet* ADesPlayerCharacter::GetAttributeSet() const
{
	return GetPlayerStateChecked<ADesPlayerState>()->GetAttributeSet();
}
