#include "Player/Ability/Grab/DesAbilityTask_PlayerGrab.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Player/DesPlayerCharacter.h"

UDesAbilityTask_PlayerGrab::UDesAbilityTask_PlayerGrab(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	bTickingTask = true;
}

void UDesAbilityTask_PlayerGrab::TickTask(float DeltaTime)
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

UDesAbilityTask_PlayerGrab* UDesAbilityTask_PlayerGrab::PlayerGrab(UGameplayAbility* OwningAbility,
                                                                 ADesPlayerCharacter* InCharacter)
{
	const auto NewTask = NewAbilityTask<UDesAbilityTask_PlayerGrab>(OwningAbility);
	NewTask->Character = MakeWeakObjectPtr(InCharacter);
	return NewTask;
}
