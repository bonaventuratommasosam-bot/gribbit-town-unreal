#include "GribbitTownGameMode.h"
#include "GribbitCharacter.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/UObjectGlobals.h"

/**
 * GribbitTownGameMode.cpp
 *
 * Current implementation:
 * - Spawns the player Gribbit (AGribbitCharacter)
 * - Procedurally builds a basic closed town using primitive shapes
 *   (for fast prototyping and testing movement/camera)
 *
 * Design alignment (from collaborative sessions):
 * - This procedural approach is temporary for early testing.
 * - Long-term goal: Move to manual level design + World Partition / Level Streaming.
 * - Prepare systems for the 7 iconic characters (Chill Pete, Sheriff Buck, Max MARFA, etc.)
 * - Foundation for Needs system, Outfit system, and future multiplayer.
 */

// Spawn a basic engine shape with a solid color. Safe if the mesh is missing.
static AStaticMeshActor* SpawnShape(UWorld* World, const TCHAR* MeshPath, const FVector& Loc, const FRotator& Rot, const FVector& Scale, const FLinearColor& Color)
{
	if (!World) return nullptr;
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, MeshPath);
	if (!Mesh) return nullptr;

	AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, Rot);
	if (!Actor) return nullptr;

	UStaticMeshComponent* Smc = Actor->GetStaticMeshComponent();
	Smc->SetStaticMesh(Mesh);
	Actor->SetActorScale3D(Scale);
	Smc->SetMobility(EComponentMobility::Static);

	UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (BaseMat)
	{
		UMaterialInstanceDynamic* Dyn = UMaterialInstanceDynamic::Create(BaseMat, Actor);
		if (Dyn)
		{
			Dyn->SetVectorParameterValue(FName("Color"), Color);
			Smc->SetMaterial(0, Dyn);
		}
	}
	return Actor;
}

AGribbitTownGameMode::AGribbitTownGameMode()
{
	DefaultPawnClass = AGribbitCharacter::StaticClass();
}

void AGribbitTownGameMode::StartPlay()
{
	Super::StartPlay();

	UWorld* World = GetWorld();
	if (!World) return;

	// --- Lighting ---
	ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector(0.f, 0.f, 600.f), FRotator(-45.f, 0.f, 0.f));
	if (Sun)
	{
		Sun->GetLightComponent()->SetIntensity(3.f);
		Sun->SetMobility(EComponentMobility::Static);
	}

	ASkyLight* Sky = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(), FVector(0.f, 0.f, 600.f), FRotator::ZeroRotator);
	if (Sky)
	{
		Sky->GetLightComponent()->SetIntensity(1.f);
		Sky->SetMobility(EComponentMobility::Static);
		Sky->bCapturedScene = true;
	}

	// --- Ground ---
	SpawnShape(World, TEXT("/Engine/BasicShapes/Plane"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), FVector(50.f, 50.f, 1.f), FLinearColor(0.20f, 0.55f, 0.25f));

	// --- Central fountain ---
	SpawnShape(World, TEXT("/Engine/BasicShapes/Cylinder"), FVector(0.f, 0.f, 50.f),  FRotator::ZeroRotator, FVector(180.f, 180.f, 50.f),  FLinearColor(0.75f, 0.75f, 0.78f));
	SpawnShape(World, TEXT("/Engine/BasicShapes/Sphere"),   FVector(0.f, 0.f, 130.f), FRotator::ZeroRotator, FVector(110.f, 110.f, 80.f),  FLinearColor(0.25f, 0.55f, 0.85f));
	SpawnShape(World, TEXT("/Engine/BasicShapes/Cylinder"), FVector(0.f, 0.f, 200.f), FRotator::ZeroRotator, FVector(25.f, 25.f, 200.f),  FLinearColor(0.65f, 0.65f, 0.70f));

	// --- 6 Buildings ---
	struct BInfo { FVector Pos; FVector Scale; FLinearColor Col; };
	BInfo Buildings[6] = {
		{ FVector(  0.f,  600.f, 300.f), FVector(120.f, 120.f, 300.f), FLinearColor(0.85f, 0.40f, 0.30f) },
		{ FVector( 600.f,  200.f, 220.f), FVector(110.f, 110.f, 220.f), FLinearColor(0.30f, 0.55f, 0.85f) },
		{ FVector( 450.f, -500.f, 360.f), FVector(100.f, 100.f, 360.f), FLinearColor(0.90f, 0.80f, 0.30f) },
		{ FVector(-450.f, -400.f, 260.f), FVector(130.f, 130.f, 260.f), FLinearColor(0.55f, 0.35f, 0.75f) },
		{ FVector(-550.f,  350.f, 320.f), FVector(115.f, 115.f, 320.f), FLinearColor(0.35f, 0.80f, 0.55f) },
		{ FVector( 100.f, -750.f, 200.f), FVector(140.f, 140.f, 200.f), FLinearColor(0.80f, 0.50f, 0.70f) },
	};
	for (auto& B : Buildings)
	{
		SpawnShape(World, TEXT("/Engine/BasicShapes/Cube"), B.Pos, FRotator::ZeroRotator, B.Scale, B.Col);
	}

	// --- Trees ---
	struct TInfo { FVector Pos; };
	TInfo Trees[8] = {
		{ FVector( 300.f,  300.f, 0.f) }, { FVector(-300.f,  300.f, 0.f) },
		{ FVector( 300.f, -300.f, 0.f) }, { FVector(-300.f, -300.f, 0.f) },
		{ FVector( 800.f,  800.f, 0.f) }, { FVector(-800.f, -800.f, 0.f) },
		{ FVector( 800.f, -800.f, 0.f) }, { FVector(-800.f,  800.f, 0.f) },
	};
	for (auto& T : Trees)
	{
		SpawnShape(World, TEXT("/Engine/BasicShapes/Cylinder"), T.Pos + FVector(0.f, 0.f, 90.f), FRotator::ZeroRotator, FVector(20.f, 20.f, 90.f), FLinearColor(0.40f, 0.25f, 0.12f));
		SpawnShape(World, TEXT("/Engine/BasicShapes/Cone"),     T.Pos + FVector(0.f, 0.f, 220.f), FRotator::ZeroRotator, FVector(60.f, 60.f, 120.f), FLinearColor(0.15f, 0.60f, 0.20f));
	}

	// --- Temporary Frog NPCs (green spheres) ---
	// TODO: Replace with proper AGribbitCharacter instances using the 7 iconic characters
	FVector FrogSpots[10] = {
		FVector(  50.f,  100.f, 60.f), FVector( 250.f,  -50.f, 60.f),
		FVector(-150.f,  200.f, 60.f), FVector( 400.f,  300.f, 60.f),
		FVector(-350.f, -100.f, 60.f), FVector( 150.f,  500.f, 60.f),
		FVector(-200.f, -400.f, 60.f), FVector( 550.f, -200.f, 60.f),
		FVector(-500.f,  150.f, 60.f), FVector( 100.f, -550.f, 60.f),
	};
	for (auto& F : FrogSpots)
	{
		SpawnShape(World, TEXT("/Engine/BasicShapes/Sphere"), F, FRotator::ZeroRotator, FVector(60.f, 60.f, 60.f), FLinearColor(0.30f, 0.85f, 0.35f));
	}

	// --- Closed-world perimeter ---
	const float Half = 1200.f;
	struct WInfo { FVector Pos; FVector Scale; };
	WInfo Walls[4] = {
		{ FVector(   0.f,  Half, 150.f), FVector(2.f * Half, 20.f, 300.f) },
		{ FVector(   0.f, -Half, 150.f), FVector(2.f * Half, 20.f, 300.f) },
		{ FVector(  Half,   0.f, 150.f), FVector(20.f, 2.f * Half, 300.f) },
		{ FVector( -Half,   0.f, 150.f), FVector(20.f, 2.f * Half, 300.f) },
	};
	for (auto& W : Walls)
	{
		SpawnShape(World, TEXT("/Engine/BasicShapes/Cube"), W.Pos, FRotator::ZeroRotator, W.Scale, FLinearColor(0.45f, 0.45f, 0.50f));
	}
}
