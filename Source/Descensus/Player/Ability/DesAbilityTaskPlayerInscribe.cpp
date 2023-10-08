#include "Player/Ability/DesAbilityTaskPlayerInscribe.h"

#include "DesLogging.h"
#include "Character/DesInscriptionComponent.h"
#include "Player/DesPlayerController.h"
#include "Player/DesInscriptionCanvas.h"

void UDesAbilityTaskPlayerInscribe::Activate()
{
	if (const auto World = GetWorld())
	{
		PerformAction();
		World->GetTimerManager().SetTimer(PerformActionTimerHandle, this,
		                                  &UDesAbilityTaskPlayerInscribe::PerformAction,
		                                  0.05f, true);
	}

	InscriptionCanvas->StartInscribing();

	Super::Activate();
}

void UDesAbilityTaskPlayerInscribe::PerformAction() const
{
	FVector2D Position;
	FVector2D PositionNDC;
	FVector2D PositionNormalized;
	PlayerController->GetMousePositionDetailed(Position, PositionNDC, PositionNormalized);

	InscriptionComponent->SetInscriptionOffset(PositionNDC);

	InscriptionCanvas->AddPoint(Position, PositionNormalized);
}

void UDesAbilityTaskPlayerInscribe::OnDestroy(bool bInOwnerFinished)
{
	if (const auto World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PerformActionTimerHandle);
	}

	InscriptionCanvas->StopInscribing();

	Super::OnDestroy(bInOwnerFinished);
}

UDesAbilityTaskPlayerInscribe* UDesAbilityTaskPlayerInscribe::PlayerInscribe(UGameplayAbility* OwningAbility,
                                                                             UDesInscriptionComponent*
                                                                             InInscriptionComponent,
                                                                             ADesInscriptionCanvas* InInscriptionCanvas,
                                                                             ADesPlayerController* InPlayerController)
{
	UDesAbilityTaskPlayerInscribe* MyObj = NewAbilityTask<UDesAbilityTaskPlayerInscribe>(OwningAbility);
	MyObj->InscriptionComponent = MakeWeakObjectPtr(InInscriptionComponent);
	MyObj->InscriptionCanvas = MakeWeakObjectPtr(InInscriptionCanvas);
	MyObj->PlayerController = MakeWeakObjectPtr(InPlayerController);
	return MyObj;
}
