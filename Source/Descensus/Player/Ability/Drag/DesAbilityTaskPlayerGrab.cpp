#include "Player/Ability/Drag/DesAbilityTaskPlayerGrab.h"
#include "Character/DesCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UDesAbilityTaskPlayerGrab::UDesAbilityTaskPlayerGrab(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	bTickingTask = true;
}

void UDesAbilityTaskPlayerGrab::TickTask(float DeltaTime)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!PhysicsHandle.IsValid() || !Character.IsValid())
		{
			return;
		}
		FVector PhysicsHandleLocation;
		FRotator PhysicsHandleRotation;
		PhysicsHandle->GetTargetLocationAndRotation(PhysicsHandleLocation, PhysicsHandleRotation);
		FVector EyesLocation;
		FRotator EyesRotation;
		Character->GetActorEyesViewPoint(EyesLocation, EyesRotation);
		EyesLocation += EyesRotation.Vector() * 150.0f;
		PhysicsHandle->SetTargetLocation(FMath::InterpEaseInOut(PhysicsHandleLocation, EyesLocation, DeltaTime * 35.0f, 2.0f));
	}
}
