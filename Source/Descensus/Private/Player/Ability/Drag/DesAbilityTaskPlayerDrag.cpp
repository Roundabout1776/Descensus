#include "Player/Ability/Drag/DesAbilityTaskPlayerDrag.h"
#include "DesLogging.h"

UDesAbilityTaskPlayerDrag::UDesAbilityTaskPlayerDrag(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = true;
}

void UDesAbilityTaskPlayerDrag::Activate()
{
	SetWaitingOnAvatar();
}

void UDesAbilityTaskPlayerDrag::TickTask(float DeltaTime)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
        DES_LOG_BOOL("PlayerDragTask: ", true)
	}
}

