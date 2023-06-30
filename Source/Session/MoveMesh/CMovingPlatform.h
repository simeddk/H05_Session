#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CMovingPlatform.generated.h"

UCLASS()
class SESSION_API ACMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ACMovingPlatform();

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		float Speed = 300.f;
};
