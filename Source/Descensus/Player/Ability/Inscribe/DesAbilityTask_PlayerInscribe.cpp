#include "Player/Ability/Inscribe/DesAbilityTask_PlayerInscribe.h"

#include "DesLogging.h"
#include "Character/DesInscriptionComponent.h"
#include "Player/DesPlayerController.h"
#include "Player/DesInscriptionCanvas.h"

void UDesAbilityTask_PlayerInscribe::Activate()
{
	if (const auto World = GetWorld())
	{
		PerformAction();
		World->GetTimerManager().SetTimer(PerformActionTimerHandle, this,
		                                  &UDesAbilityTask_PlayerInscribe::PerformAction,
		                                  0.05f, true);
	}

	InscriptionCanvas->StartInscribing();

	Super::Activate();
}

void UDesAbilityTask_PlayerInscribe::PerformAction() const
{
	FVector2D Position;
	FVector2D PositionNDC;
	FVector2D PositionNormalized;
	PlayerController->GetMousePositionDetailed(Position, PositionNDC, PositionNormalized);

	InscriptionComponent->SetInscriptionOffset(PositionNDC);

	InscriptionCanvas->AddPoint(Position, PositionNormalized);
}

void UDesAbilityTask_PlayerInscribe::OnDestroy(bool bInOwnerFinished)
{
	if (const auto World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PerformActionTimerHandle);
	}

	InscriptionCanvas->StopInscribing();

	Super::OnDestroy(bInOwnerFinished);
}

UDesAbilityTask_PlayerInscribe* UDesAbilityTask_PlayerInscribe::PlayerInscribe(UGameplayAbility* OwningAbility,
                                                                             UDesInscriptionComponent*
                                                                             InInscriptionComponent,
                                                                             ADesInscriptionCanvas* InInscriptionCanvas,
                                                                             ADesPlayerController* InPlayerController)
{
	UDesAbilityTask_PlayerInscribe* MyObj = NewAbilityTask<UDesAbilityTask_PlayerInscribe>(OwningAbility);
	MyObj->InscriptionComponent = MakeWeakObjectPtr(InInscriptionComponent);
	MyObj->InscriptionCanvas = MakeWeakObjectPtr(InInscriptionCanvas);
	MyObj->PlayerController = MakeWeakObjectPtr(InPlayerController);
	return MyObj;
}
