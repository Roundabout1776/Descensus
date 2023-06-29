#include "Player/DesHeadBobComponent.h"

#include "AbilitySystemComponent.h"
#include "DesGameplayTags.h"
#include "Player/DesPlayerCharacter.h"
#include "Player/DesPlayerController.h"
#include "MathUtil.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraShakeBase.h"

UDesHeadBobComponent::UDesHeadBobComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDesHeadBobComponent::PostLoad()
{
	Super::PostLoad();

	DesPlayerCharacterOwner = GetOwner<ADesPlayerCharacter>();
}

void UDesHeadBobComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (const auto& CameraShake : CameraShakeMap)
	{
		if (CameraShake.Value->IsActive())
		{
			CameraShake.Value->StopShake();
		}
	}

	CameraShakeMap.Empty();
}

void UDesHeadBobComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto Character = GetOwner<ADesPlayerCharacter>();
	check(Character);

	const auto PlayerController = Character->GetController<ADesPlayerController>();
	if (!PlayerController)
	{
		return;
	}

	if (IdleCameraShakeClass)
	{
		CameraShakeMap.Add(EHeadBobMode::Idle,
		                   PlayerController->PlayerCameraManager->StartCameraShake(
			                   IdleCameraShakeClass, TMathUtilConstants<float>::Epsilon));
	}
	if (WalkCameraShakeClass)
	{
		CameraShakeMap.Add(EHeadBobMode::Walk,
		                   PlayerController->PlayerCameraManager->StartCameraShake(
			                   WalkCameraShakeClass, TMathUtilConstants<float>::Epsilon));
	}
	if (RunCameraShakeClass)
	{
		CameraShakeMap.Add(EHeadBobMode::Run,
		                   PlayerController->PlayerCameraManager->StartCameraShake(
			                   RunCameraShakeClass, TMathUtilConstants<float>::Epsilon));
	}
}

void UDesHeadBobComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CameraShakeMap.Num() == 0)
	{
		return;
	}

	auto HeadBobMode = EHeadBobMode::Idle;

	if (DesPlayerCharacterOwner->GetCharacterMovement()->Velocity.SquaredLength() > 0.1)
	{
		if (DesPlayerCharacterOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag((TAG_Movement_Run)))
		{
			HeadBobMode = EHeadBobMode::Run;
		}
		else
		{
			HeadBobMode = EHeadBobMode::Walk;
		}
	}

	for (const auto& CameraShake : CameraShakeMap)
	{
		CameraShake.Value->ShakeScale = FMath::FInterpConstantTo(CameraShake.Value->ShakeScale,
		                                                         CameraShake.Key == HeadBobMode ? 1.0 : 0.0, DeltaTime,
		                                                         2.5);
	}
}
