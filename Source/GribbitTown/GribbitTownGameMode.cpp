#include "GribbitTownGameMode.h"
#include "GribbitCharacter.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

AGribbitTownGameMode::AGribbitTownGameMode()
{
	DefaultPawnClass = AGribbitCharacter::StaticClass();
}

void AGribbitTownGameMode::StartPlay()
{
	Super::StartPlay();

	UWorld* World = GetWorld();
	if (!World) return;

	// Basic lighting
	if (ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector(0,0,600), FRotator(-45,0,0)))
	{
		Sun->GetLightComponent()->SetIntensity(3.f);
	}

	if (ASkyLight* Sky = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(), FVector(0,0,600), FRotator::ZeroRotator))
	{
		Sky->GetLightComponent()->SetIntensity(1.f);
	}

	// Spawn a few initial objects so the world isn't empty
	FVector StartLoc = FVector(200, 0, 50);
	for (int i = 0; i < 5; i++)
	{
		if (AStaticMeshActor* Cube = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), StartLoc + FVector(i * 150, 0, 0), FRotator::ZeroRotator))
		{
			UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
			if (Mesh) Cube->GetStaticMeshComponent()->SetStaticMesh(Mesh);
		}
	}
}
