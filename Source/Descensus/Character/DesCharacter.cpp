#include "Character/DesCharacter.h"

#include "DesLogging.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"
#include "Actor/DesMetaComponent.h"
#include "Character/Ability/Jump/DesGameplayAbilityJump.h"
#include "Character/Ability/Run/DesGameplayAbilityRun.h"
#include "Character/Ability/DesGameplayEffectStaminaRegen.h"
#include "Components/AudioComponent.h"
#include "Character/DesCharacterAttributeSet.h"
#include "Character/DesCharacterMovementComponent.h"
#include "Character/DesInscriptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/DesAbilityInputID.h"
#include "Player/DesPlayerState.h"

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
