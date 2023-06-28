#include "Character/Shared/Ability/Run/DesAbilityTaskRun.h"

#include "Character/DesCharacterMovementComponent.h"

UDesAbilityTaskRun::UDesAbilityTaskRun(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = true;
}

UDesAbilityTaskRun* UDesAbilityTaskRun::Run(UGameplayAbility* OwningAbility)
{
	UDesAbilityTaskRun* MyObj = NewAbilityTask<UDesAbilityTaskRun>(OwningAbility);
	return MyObj;
}

void UDesAbilityTaskRun::Activate()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	CachedMovementComponent = MakeWeakObjectPtr(
		Cast<UDesCharacterMovementComponent>(ActorInfo->MovementComponent.Get()));
	SetWaitingOnAvatar();
}

void UDesAbilityTaskRun::TickTask(float DeltaTime)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnRun.ExecuteIfBound(CachedMovementComponent.Get()->IsEligibleForRunning(), DeltaTime);
	}
}
