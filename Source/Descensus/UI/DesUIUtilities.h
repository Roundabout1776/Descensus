#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DesUIUtilities.generated.h"

UCLASS()
class DESCENSUS_API UDesUIUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static void SamplePointOnBounds(APlayerController* PlayerController, FVector Point, FVector2D& TopLeftOut,
	                                FVector2D& BottomRightOut);

	UFUNCTION(BlueprintCallable)
	static void GetOrientedBounds(APlayerController* PlayerController, const AActor* Actor, bool bNonColliding,
	                              bool bIncludeFromChildActors,
	                              FVector2D& TopLeftOut, FVector2D& BottomRightOut);

public:
	UFUNCTION(BlueprintCallable)
	static void GetActorBoundsOnScreen(APlayerController* PlayerController, const AActor* Actor, FVector2D& TopLeftOut,
	                                   FVector2D& BottomRightOut);
};
