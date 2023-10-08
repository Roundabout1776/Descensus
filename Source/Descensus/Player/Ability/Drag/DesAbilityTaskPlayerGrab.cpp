#include "Player/Ability/Drag/DesAbilityTaskPlayerGrab.h"

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
		FVector PhysicsHandleLocation;
		FRotator PhysicsHandleRotation;
		Character->PhysicsHandle->GetTargetLocationAndRotation(PhysicsHandleLocation, PhysicsHandleRotation);
		FVector EyesLocation;
		FRotator EyesRotation;
		Character->GetActorEyesViewPoint(EyesLocation, EyesRotation);
		EyesLocation += EyesRotation.Vector() * 150.0f;
		Character->PhysicsHandle->SetTargetLocation(FMath::InterpEaseInOut(PhysicsHandleLocation, EyesLocation, DeltaTime * 35.0f, 2.0f));
	}
}
