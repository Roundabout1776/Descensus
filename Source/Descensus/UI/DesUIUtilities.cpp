#include "UI/DesUIUtilities.h"

#include "Blueprint/WidgetLayoutLibrary.h"
// #include "Kismet/KismetSystemLibrary.h"

void UDesUIUtilities::GetActorBoundsOnScreen(APlayerController* PlayerController, const AActor* Actor,
                                             FVector2D& TopLeftOut, FVector2D& BottomRightOut)
{
	TopLeftOut = FVector2D(TNumericLimits<double>::Max(), TNumericLimits<double>::Max());
	BottomRightOut = FVector2D(TNumericLimits<double>::Min(), TNumericLimits<double>::Min());

	GetOrientedBounds(PlayerController, Actor, false, false, TopLeftOut, BottomRightOut);
}

void UDesUIUtilities::SamplePointOnBounds(APlayerController* PlayerController, FVector Point, FVector2D& TopLeftOut,
                                          FVector2D& BottomRightOut)
{
	if (FVector2D Temp; UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, Point, Temp, true))
	{
		TopLeftOut.X = FMath::Min(TopLeftOut.X, Temp.X);
		TopLeftOut.Y = FMath::Min(TopLeftOut.Y, Temp.Y);
		BottomRightOut.X = FMath::Max(BottomRightOut.X, Temp.X);
		BottomRightOut.Y = FMath::Max(BottomRightOut.Y, Temp.Y);
	}
	// UKismetSystemLibrary::DrawDebugSphere(PlayerController, Point, 5.0f, 12);
}

void UDesUIUtilities::GetOrientedBounds(APlayerController* PlayerController, const AActor* Actor, bool bNonColliding,
                                        bool bIncludeFromChildActors,
                                        FVector2D& TopLeftOut, FVector2D& BottomRightOut)
{
	Actor->ForEachComponent<USceneComponent>(bIncludeFromChildActors, [&](const USceneComponent* InSceneComponent)
	{
		if (InSceneComponent->IsRegistered() && (bNonColliding || InSceneComponent->IsCollisionEnabled()))
		{
			const auto Origin = InSceneComponent->Bounds.Origin;
			auto BoxExtent = InSceneComponent->GetLocalBounds().BoxExtent;
			FTransform Transform(FRotator::ZeroRotator, FVector::Zero(), InSceneComponent->GetRelativeScale3D());
			BoxExtent = Transform.TransformPosition(BoxExtent);
			if (Actor->GetRootComponent() == InSceneComponent)
			{
				Transform = FTransform(InSceneComponent->GetComponentRotation(), FVector::Zero(),
				                       FVector::One());
			}
			else
			{
				Transform = FTransform(InSceneComponent->GetComponentRotation(), FVector::Zero(),
				                       Actor->GetActorScale3D());
			}

			SamplePointOnBounds(PlayerController,
			                    Origin + Transform.TransformPosition(FVector(BoxExtent.X, BoxExtent.Y, BoxExtent.Z)),
			                    TopLeftOut,
			                    BottomRightOut);
			SamplePointOnBounds(PlayerController,
			                    Origin + Transform.TransformPosition(FVector(-BoxExtent.X, BoxExtent.Y, BoxExtent.Z)),
			                    TopLeftOut,
			                    BottomRightOut);
			SamplePointOnBounds(PlayerController,
			                    Origin + Transform.TransformPosition(FVector(BoxExtent.X, -BoxExtent.Y, BoxExtent.Z)),
			                    TopLeftOut,
			                    BottomRightOut);
			SamplePointOnBounds(PlayerController,
			                    Origin + Transform.TransformPosition(FVector(-BoxExtent.X, -BoxExtent.Y, BoxExtent.Z)),
			                    TopLeftOut,
			                    BottomRightOut);
			SamplePointOnBounds(PlayerController,
			                    Origin + Transform.TransformPosition(FVector(BoxExtent.X, BoxExtent.Y, -BoxExtent.Z)),
			                    TopLeftOut,
			                    BottomRightOut);
			SamplePointOnBounds(PlayerController,
			                    Origin + Transform.TransformPosition(FVector(-BoxExtent.X, BoxExtent.Y, -BoxExtent.Z)),
			                    TopLeftOut,
			                    BottomRightOut);
			SamplePointOnBounds(PlayerController,
			                    Origin + Transform.TransformPosition(FVector(BoxExtent.X, -BoxExtent.Y, -BoxExtent.Z)),
			                    TopLeftOut,
			                    BottomRightOut);
			SamplePointOnBounds(PlayerController,
			                    Origin + Transform.TransformPosition(FVector(-BoxExtent.X, -BoxExtent.Y, -BoxExtent.Z)),
			                    TopLeftOut,
			                    BottomRightOut);
		}
	});
}
