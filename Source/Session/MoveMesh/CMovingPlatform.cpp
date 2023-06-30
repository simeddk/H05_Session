#include "CMovingPlatform.h"

ACMovingPlatform::ACMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer'"));
	if (meshAsset.Succeeded())
		GetStaticMeshComponent()->SetStaticMesh(meshAsset.Object);

	GetStaticMeshComponent()->SetRelativeScale3D(FVector(2.f, 2.f, 0.5f));

	SetMobility(EComponentMobility::Movable);
}

void ACMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void ACMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector location = GetActorLocation();
		location += FVector(Speed * DeltaTime, 0, 0);

		SetActorLocation(location);
	}
}
