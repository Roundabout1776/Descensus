#include "Character/Ability/Run/DesAbilityTask_Run.h"

#include "Character/DesCharacterMovementComponent.h"

UDesAbilityTask_Run::UDesAbilityTask_Run(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = true;
}

void UDesAbilityTask_Run::Activate()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	CachedMovementComponent = MakeWeakObjectPtr(
		Cast<UDesCharacterMovementComponent>(ActorInfo->MovementComponent.Get()));
	SetWaitingOnAvatar();
}

void UDesAbilityTask_Run::TickTask(float DeltaTime)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnRun.ExecuteIfBound(CachedMovementComponent.Get()->IsEligibleForRunning(), DeltaTime);
	}
}
