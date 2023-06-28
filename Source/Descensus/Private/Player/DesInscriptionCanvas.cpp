#include "Player/DesInscriptionCanvas.h"

#include "DesAssetManager.h"
#include "DesLogging.h"
#include "NiagaraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Magic/DesInscriptionSubsystem.h"
#include "Magic/DesRuneData.h"

ADesInscriptionCanvas::ADesInscriptionCanvas()
{
	PrimaryActorTick.bCanEverTick = false;
	// PrimaryActorTick.TickInterval = 0.1f;

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("InscriptionCanvasCapture");
	Capture->ProjectionType = ECameraProjectionMode::Orthographic;
	SetRootComponent(Capture);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>("InscriptionCanvasBrush");
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetAutoDestroy(false);
	Niagara->SetAutoActivate(false);
	Niagara->SetVisibleInSceneCaptureOnly(true);

	Capture->ShowOnlyComponent(Niagara);
}

void ADesInscriptionCanvas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADesInscriptionCanvas::DoInitialSetup(const FViewport* Viewport)
{
	Texture = NewObject<UTextureRenderTarget2D>(this);
	Texture->bAutoGenerateMips = false;
	Texture->ResizeTarget(Viewport->GetSizeXY().X, Viewport->GetSizeXY().Y);
	Viewport->ViewportResizedEvent.AddWeakLambda(
		this, [this](const FViewport* InViewport, uint32 _)
		{
			if (!Texture)
			{
				return;
			}

			Texture->ResizeTarget(InViewport->GetSizeXY().X, InViewport->GetSizeXY().Y);
		});

	Material = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	Material->SetFlags(RF_Transient);
	Material->SetTextureParameterValue("OverlayTexture", Texture);

	Capture->TextureTarget = Texture;

	const auto& AssetManager = UDesAssetManager::Get();
	auto& StreamableManager =
		AssetManager.GetStreamableManager();
	TArray<FAssetData> AssetReferences;
	AssetManager.GetPrimaryAssetDataList(UDesAssetManager::RuneDataType, AssetReferences);
	for (const auto& AssetData : AssetReferences)
	{
		const auto RuneData = Cast<UDesRuneData>(StreamableManager.LoadSynchronous(AssetData.GetSoftObjectPath()));
		Runes.Add(RuneData);
		DES_LOG(Warning, TEXT("Loaded RuneData: %s, Template Count: %i"), *RuneData->GameplayTag.ToString(), RuneData->Templates.Num());
	}
}

void ADesInscriptionCanvas::StartInscribing()
{
	Niagara->Activate(false);
	Gesture.PointsRaw.Reset();
}

void ADesInscriptionCanvas::StopInscribing()
{
	Niagara->Deactivate();
	if (Gesture.UpdateFromPointsRaw())
	{
		double Score = 0.0;
		if (const auto RuneData = GEngine->GetEngineSubsystem<UDesInscriptionSubsystem>()->Recognize(
			Gesture, Runes, Score))
		{
			UE_LOG(LogTemp, Warning, TEXT("Rune: %s, Score: %f"), *RuneData->GameplayTag.ToString(), Score);
		}
	}
}

void ADesInscriptionCanvas::AddPoint(const FVector2D& InPosition, const FVector2D& InNormalizedPosition)
{
	const auto Position = InNormalizedPosition * Capture->OrthoWidth / 2.0;

	Gesture.PointsRaw.Add(FDesStrokePoint(InPosition));

	Niagara->SetRelativeLocation(FVector(0.0, Position.X, Position.Y));
}
