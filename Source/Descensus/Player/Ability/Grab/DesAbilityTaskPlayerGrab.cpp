#include "Player/Ability/Grab/DesAbilityTaskPlayerGrab.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Player/DesPlayerCharacter.h"

UDesAbilityTaskPlayerGrab::UDesAbilityTaskPlayerGrab(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	bTickingTask = true;
}

void UDesAbilityTaskPlayerGrab::TickTask(float DeltaTime)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!Character.IsValid())
		{
			return;
		}
		FVector EyesLocation;
		FRotator EyesRotation;
		Character->GetActorEyesViewPoint(EyesLocation, EyesRotation);
		EyesLocation += EyesRotation.Vector() * 150.0f;
		Character->PhysicsHandle->SetTargetLocation(EyesLocation);
	}
}

UDesAbilityTaskPlayerGrab* UDesAbilityTaskPlayerGrab::PlayerGrab(UGameplayAbility* OwningAbility,
                                                                 ADesPlayerCharacter* InCharacter)
{
	const auto NewTask = NewAbilityTask<UDesAbilityTaskPlayerGrab>(OwningAbility);
	NewTask->Character = MakeWeakObjectPtr(InCharacter);
	return NewTask;
}
