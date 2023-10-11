#include "DesWaterVolume.h"

#if WITH_EDITOR
#include "ActorFactories/ActorFactory.h"
#include "Builders/CubeBuilder.h"
#endif
#include "Components/BrushComponent.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/PhysicsVolume.h"

ADesWaterVolume::ADesWaterVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	PostProcessVolumeChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("PostProcessVolumeChildActor"));
	PostProcessVolumeChildActor->SetChildActorClass(APostProcessVolume::StaticClass());
	PostProcessVolumeChildActor->SetupAttachment(RootComponent);

	PhysicsVolumeChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("PhysicsVolumeChildActor"));
	PhysicsVolumeChildActor->SetChildActorClass(APhysicsVolume::StaticClass());
	PhysicsVolumeChildActor->SetupAttachment(RootComponent);
}

void ADesWaterVolume::BeginPlay()
{
	Super::BeginPlay();
}

void ADesWaterVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void InitBrushForVolumeActor(AVolume* VolumeActor)
{
	check(IsValid(VolumeActor))

	if (!IsValid(VolumeActor->BrushBuilder))
	{
		const auto Builder = NewObject<UCubeBuilder>();
		Builder->X = 100.0f;
		Builder->Y = 100.0f;
		Builder->Z = 100.0f;
		UActorFactory::CreateBrushForVolumeActor(VolumeActor, Builder);
	}

	const auto BrushComponent = VolumeActor->GetBrushComponent();
	BrushComponent->BuildSimpleBrushCollision();
	if (BrushComponent->IsPhysicsStateCreated())
	{
		BrushComponent->RecreatePhysicsState();
	}

	// BrushComponent->SetCollisionResponseToAllChannels(ECR_Block);
	// BrushComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	// BrushComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}
#endif

void ADesWaterVolume::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

#if WITH_EDITOR
	InitBrushForVolumeActor(Cast<APostProcessVolume>(PostProcessVolumeChildActor->GetChildActor()));
	InitBrushForVolumeActor(Cast<APhysicsVolume>(PhysicsVolumeChildActor->GetChildActor()));
#endif
}
