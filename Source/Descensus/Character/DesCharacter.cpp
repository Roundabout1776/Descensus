#include "Character/DesCharacter.h"

#include "DesLogging.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"
#include "Actor/DesMetaComponent.h"
#include "Character/Shared/Ability/Jump/DesGameplayAbilityJump.h"
#include "Character/Shared/Ability/Run/DesGameplayAbilityRun.h"
#include "Character/Shared/DesGameplayEffectStaminaRegen.h"
#include "Components/AudioComponent.h"
#include "Character/DesCharacterAttributeSet.h"
#include "Character/DesCharacterMovementComponent.h"
#include "Character/DesInscriptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/DesAbilityInputID.h"

FName ADesCharacter::AttributeSetName(TEXT("CharacterAttributeSet"));

ADesCharacter::ADesCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UDesCharacterMovementComponent>(
		CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;

	AttributeSet = CreateDefaultSubobject<UDesCharacterAttributeSet>(AttributeSetName);

	AbilitySystemComponent = CreateDefaultSubobject<UDesAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

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

void ADesCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		GiveDefaultAbilities();

		/* Add default effects */
		const auto Context = AbilitySystemComponent->MakeEffectContext();
		for (const auto& EffectClass : DefaultEffects)
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
				*AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 0.0, Context).Data.Get(), FPredictionKey());
		}
	}

	SetOwner(NewController);
}

UAbilitySystemComponent* ADesCharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void ADesCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADesCharacter::GiveDefaultAbilities()
{
	for (const auto& AbilityClass : DefaultAbilities)
	{
		GiveAbility(AbilityClass);
	}
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
	AbilitySystemComponent->GiveAbility(AbilitySpec);
}
