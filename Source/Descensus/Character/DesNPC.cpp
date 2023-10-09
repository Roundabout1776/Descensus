#include "DesNPC.h"

#include "DesCharacterAttributeSet.h"
#include "AbilitySystem/DesAbilitySystemComponent.h"

ADesNPC::ADesNPC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttributeSet = CreateDefaultSubobject<UDesCharacterAttributeSet>(UDesCharacterAttributeSet::AttributeSetName);

	ASC = CreateDefaultSubobject<UDesAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	CustomASC = MakeWeakObjectPtr(ASC);
}

void ADesNPC::BeginPlay()
{
	ASC->InitAbilityActorInfo(this, this);

	GiveDefaultAbilities();
	ApplyDefaultEffects();
	
	Super::BeginPlay();
}

void ADesNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADesNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* ADesNPC::GetAbilitySystemComponent() const
{
	return ASC;
}

UAttributeSet* ADesNPC::GetAttributeSet() const
{
	return AttributeSet;
}
