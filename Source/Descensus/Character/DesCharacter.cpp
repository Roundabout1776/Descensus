#include "Character/DesCharacter.h"

#include "DesPhysicalMaterial.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"
#include "Components/DesMetaComponent.h"
#include "Character/Ability/Jump/DesGameplayAbilityJump.h"
#include "Character/Ability/Run/DesGameplayAbilityRun.h"
#include "Character/Ability/DesGameplayEffectStaminaRegen.h"
#include "Components/AudioComponent.h"
#include "Character/DesCharacterAttributeSet.h"
#include "Character/DesCharacterMovementComponent.h"
#include "Character/DesInscriptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/DesAbilityInputID.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<int32> CVarShowFootsteps(
	TEXT("ShowDebugFootsteps"),
	0,
	TEXT("Show debug footsteps"),
	ECVF_Cheat
);

ADesCharacter::ADesCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UDesCharacterMovementComponent>(
		CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;

	DefaultAbilities.Empty();
	DefaultAbilities.Add(UDesGameplayAbilityRun::StaticClass());
	DefaultAbilities.Add(UDesGameplayAbilityJump::StaticClass());

	DefaultEffects.Empty();
	DefaultEffects.Add(UDesGameplayEffectStaminaRegen::StaticClass());

	CommonAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CommonAudioComponent"));
	CommonAudio->SetupAttachment(GetRootComponent());
	CommonAudio->bAutoActivate = false;

	InscriptionComponent = CreateDefaultSubobject<UDesInscriptionComponent>(TEXT("InscriptionComponent"));
	InscriptionComponent->SetIsReplicated(true);

	MetaComponent = CreateDefaultSubobject<UDesMetaComponent>(TEXT("MetaComponent"));

	const auto Movement = GetCharacterMovement();
	Movement->AirControl = 0.1;

	GetMesh()->bEnablePerPolyCollision = true;
}

void ADesCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADesCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsNetMode(NM_DedicatedServer))
		return;

	TickStepSound(DeltaTime);
}

void ADesCharacter::TickStepSound(float DeltaTime)
{
	if (!StepSound.bEnableStepSound)
		return;

	StepSound.Time = FMath::Max(0, StepSound.Time - (1000.0f * DeltaTime));
	if (StepSound.Time > 0)
		return;

	if (!GetCharacterMovement()->IsMovingOnGround())
		return;

	const auto Speed = FVector::DistXY(FVector::ZeroVector, GetCharacterMovement()->Velocity);
	const auto bIsMovingFastEnough = Speed >= StepSound.MinSpeed;

	if (!bIsMovingFastEnough)
	{
		StepSound.bSkip = true;
		return;
	}

	const bool bIsRunning = Speed >= StepSound.MinRunningSpeed;

	FVector Location = GetActorLocation();
	FVector EndLocation = Location + FVector::UpVector * -150.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(this);

	const int32 ShowFootsteps = CVarShowFootsteps.GetValueOnAnyThread();

	const auto World = GetMesh()->GetWorld();

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, Location,
	                                                          EndLocation, ECC_WorldStatic,
	                                                          QueryParams))
	{
		if (!HitResult.bBlockingHit)
		{
			if (ShowFootsteps > 0)
			{
				DrawDebugLine(World, Location, EndLocation, FColor::Green, false, 4.0f, 0, 1.0f);
			}
			return;
		}
		if (const auto* Material = Cast<UDesPhysicalMaterial>(HitResult.PhysMaterial.Get()))
		{
			if (bIsRunning && CustomASC->HasMatchingGameplayTag(TAG_Movement_Run))
			{
				StepSound.Time = StepSound.TimeRunning;
			}
			else
			{
				StepSound.Time = StepSound.TimeWalking;
			}

			if (!StepSound.bSkip)
			{
				UGameplayStatics::PlaySoundAtLocation(World, Material->FootstepSound, HitResult.Location, 1.0f);
				if (ShowFootsteps > 0)
				{
					DrawDebugString(World, HitResult.ImpactPoint, GetNameSafe(Material), nullptr,
					                FColor::White, 4.0f);
				}
			}
			else
			{
				StepSound.bSkip = false;
				StepSound.Time /= 2.0f;
			}
		}
		if (ShowFootsteps > 0)
		{
			DrawDebugSphere(World, HitResult.ImpactPoint, 16, 16, FColor::Red, false, 4.0f);
		}
	}
	else
	{
		if (ShowFootsteps > 0)
		{
			DrawDebugLine(World, Location, EndLocation, FColor::Green, false, 4.0f, 0, 1.0f);
			DrawDebugSphere(World, EndLocation, 16, 16, FColor::Red, false, 4.0f);
		}
	}
}

void ADesCharacter::GiveDefaultAbilities()
{
	if (CustomASC->bDefaultAbilitiesGiven || GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	for (const auto& AbilityClass : DefaultAbilities)
	{
		if (!IsValid(AbilityClass))
			continue;
		CustomASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1,
		                                            static_cast<int32>(AbilityClass.
		                                                               GetDefaultObject()->AbilityInputID), this));
	}
	CustomASC->bDefaultAbilitiesGiven = true;
}

void ADesCharacter::ApplyDefaultEffects()
{
	if (CustomASC->bDefaultEffectsApplied || GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	for (const auto& EffectClass : DefaultEffects)
	{
		if (!IsValid(EffectClass))
			continue;

		auto EffectContextHandle = CustomASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		if (auto GameplayEffectSpecHandle = CustomASC->MakeOutgoingSpec(
			EffectClass, 1, EffectContextHandle); GameplayEffectSpecHandle.IsValid())
		{
			CustomASC->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(),
			                                           CustomASC.Get());
		}
	}
	CustomASC->bDefaultEffectsApplied = true;
}

void ADesCharacter::GiveAbility(TSubclassOf<UDesGameplayAbility> AbilityClass) const
{
	ensure(AbilityClass);
	FGameplayAbilitySpec AbilitySpec(AbilityClass);
	if (const auto AbilityInputID = AbilityClass.GetDefaultObject()->AbilityInputID; AbilityInputID !=
		EDesAbilityInputID::None)
	{
		AbilitySpec.InputID = static_cast<int32>(AbilityInputID);
	}
	GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
}
