#include "Character/DesCharacterMovementComponent.h"

#include "Character/DesCharacter.h"
#include "Character/DesCharacterAttributeSet.h"

bool UDesCharacterMovementComponent::FDesSavedMove_Character::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                             ACharacter* InCharacter,
                                                                             float MaxDelta) const
{
	const auto NewDesMove = static_cast<FDesSavedMove_Character*>(NewMove.Get());

	return NewDesMove->bWantsToRunSaved == bWantsToRunSaved && FSavedMove_Character::CanCombineWith(
		NewMove, InCharacter, MaxDelta);
}

void UDesCharacterMovementComponent::FDesSavedMove_Character::Clear()
{
	FSavedMove_Character::Clear();

	bWantsToRunSaved = 0;
}

uint8 UDesCharacterMovementComponent::FDesSavedMove_Character::GetCompressedFlags() const
{
	auto Result = Super::GetCompressedFlags();

	if (bWantsToRunSaved)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

void UDesCharacterMovementComponent::FDesSavedMove_Character::SetMoveFor(ACharacter* C, float InDeltaTime,
                                                                         const FVector& NewAccel,
                                                                         FNetworkPredictionData_Client_Character&
                                                                         ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const auto Movement = C->GetCharacterMovement<UDesCharacterMovementComponent>();

	bWantsToRunSaved = Movement->bWantsToRunSafe;
}

void UDesCharacterMovementComponent::FDesSavedMove_Character::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	const auto Movement = C->GetCharacterMovement<UDesCharacterMovementComponent>();

	Movement->bWantsToRunSafe = bWantsToRunSaved;
}

UDesCharacterMovementComponent::FDesNetworkPredictionData_Client_Character::FDesNetworkPredictionData_Client_Character(
	const UDesCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UDesCharacterMovementComponent::FDesNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return MakeShared<FDesSavedMove_Character>();
}

void UDesCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bWantsToRunSafe = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

UDesCharacterMovementComponent::UDesCharacterMovementComponent()
{
}

void UDesCharacterMovementComponent::PostLoad()
{
	Super::PostLoad();

	DesCharacterOwner = GetOwner<ADesCharacter>();
}

float UDesCharacterMovementComponent::GetMaxSpeed() const
{
	if (const auto Owner = GetOwner<ADesCharacter>())
	{
		const auto AttributeSet = Owner->GetAttributeSet();
		if (IsValid(AttributeSet))
		{
			if (bWantsToRunSafe && IsEligibleForRunning())
			{
				return AttributeSet->GetMovementSpeed() * AttributeSet->GetMovementSpeedRunMultiplier();
			}
			return AttributeSet->GetMovementSpeed();
		}
	}
	return 0.0f;
}

FNetworkPredictionData_Client* UDesCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		const auto MutableThis = const_cast<UDesCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FDesNetworkPredictionData_Client_Character(*this);
		// MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = NetworkMaxSmoothUpdateDistance;
		// MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = NetworkNoSmoothUpdateDistance;
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.0f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
	}

	return ClientPredictionData;
}

bool UDesCharacterMovementComponent::IsEligibleForRunning() const
{
	if (!PawnOwner)
	{
		return false;
	}

	const auto bMovingForward = FVector::DotProduct(PawnOwner->GetActorForwardVector(), Velocity.GetSafeNormal()) > 0.1;

	return MovementMode == MOVE_Walking && bMovingForward && !IsFalling() && GetCurrentAcceleration().SquaredLength() >
		0.0;
}
