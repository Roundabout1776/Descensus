#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magic/DesGesture.h"
#include "DesInscriptionCanvas.generated.h"

class UDesRuneData;
class ASceneCapture2D;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(Abstract)
class DESCENSUS_API ADesInscriptionCanvas : public AActor
{
	GENERATED_BODY()

	FDesGesture Gesture;
	TArray<UDesRuneData*> Runes;

	UPROPERTY(VisibleInstanceOnly, Transient, Category = "Descensus|Inscription")
	TObjectPtr<UMaterialInstanceDynamic> Material;

	UPROPERTY(VisibleInstanceOnly, Transient, Category = "Descensus|Inscription")
	TObjectPtr<UTextureRenderTarget2D> Texture;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Descensus|Inscription")
	TObjectPtr<USceneCaptureComponent2D> Capture;

	UPROPERTY(VisibleAnywhere, Category = "Descensus|Inscription")
	TObjectPtr<UNiagaraComponent> Niagara;

	UPROPERTY(EditDefaultsOnly, Category = "Descensus|Inscription")
	TObjectPtr<UMaterial> BaseMaterial;

public:
	ADesInscriptionCanvas();
	virtual void Tick(float DeltaTime) override;

	void DoInitialSetup(const FViewport* Viewport);

	void StartInscribing();
	void StopInscribing();
	void AddPoint(const FVector2D& InPosition, const FVector2D& InNormalizedPosition);

	FORCEINLINE UMaterialInstanceDynamic* GetInscriptionCanvasMaterial() const { return Material; }
};
