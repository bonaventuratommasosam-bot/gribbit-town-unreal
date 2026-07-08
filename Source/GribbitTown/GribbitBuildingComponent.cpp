#include "GribbitBuildingComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

UGribbitBuildingComponent::UGribbitBuildingComponent()
{
	// Default to a simple cube if nothing is assigned
	if (!DefaultPlaceableObject)
	{
		DefaultPlaceableObject = AStaticMeshActor::StaticClass();
	}
}

bool UGribbitBuildingComponent::PlaceObjectInFront(TSubclassOf<AActor> ObjectClass, float Distance)
{
	if (!ObjectClass) return false;

	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return false;

	UWorld* World = GetWorld();
	if (!World) return false;

	FVector Start = OwnerChar->GetActorLocation();
	FVector Forward = OwnerChar->GetActorForwardVector();
	FVector Location = Start + Forward * Distance;

	FRotator Rotation = OwnerChar->GetActorRotation();
	FRotator FinalRotation(0.f, Rotation.Yaw, 0.f);

	AActor* NewActor = World->SpawnActor<AActor>(ObjectClass, Location, FinalRotation);

	// If it's a StaticMeshActor, give it a visible cube
	if (AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(NewActor))
	{
		UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
		if (CubeMesh)
		{
			MeshActor->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
		}
	}

	return NewActor != nullptr;
}
