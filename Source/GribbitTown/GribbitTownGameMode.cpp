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

	// Lighting
	if (ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector(0,0,600), FRotator(-45,0,0)))
	{
		Sun->GetLightComponent()->SetIntensity(3.f);
	}
	if (ASkyLight* Sky = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(), FVector(0,0,600), FRotator::ZeroRotator))
	{
		Sky->GetLightComponent()->SetIntensity(1.f);
	}

	// Spawn some initial objects to make the world feel more alive
	FVector BaseLocation = FVector(300, 0, 50);

	// Cubes in a line
	for (int i = 0; i < 6; i++)
	{
		if (AStaticMeshActor* Obj = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), BaseLocation + FVector(i * 120, 0, 0), FRotator::ZeroRotator))
		{
			UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
			if (Mesh) Obj->GetStaticMeshComponent()->SetStaticMesh(Mesh);
		}
	}

	// Some cylinders and spheres for variety
	if (AStaticMeshActor* Cyl = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(200, 300, 80), FRotator::ZeroRotator))
	{
		UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder"));
		if (Mesh) Cyl->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	}

	if (AStaticMeshActor* Sph = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(400, -250, 80), FRotator::ZeroRotator))
	{
		UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
		if (Mesh) Sph->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	}
}
