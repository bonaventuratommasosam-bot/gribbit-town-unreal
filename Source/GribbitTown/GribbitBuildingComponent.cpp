#include "GribbitBuildingComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

UGribbitBuildingComponent::UGribbitBuildingComponent()
{
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

bool UGribbitBuildingComponent::RemoveObjectInFront(float Distance)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return false;

	UWorld* World = GetWorld();
	if (!World) return false;

	FVector Start = OwnerChar->GetActorLocation();
	FVector End = Start + OwnerChar->GetActorForwardVector() * Distance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerChar);

	if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->IsA(AStaticMeshActor::StaticClass()))
		{
			HitActor->Destroy();
			return true;
		}
	}

	DrawDebugLine(World, Start, End, FColor::Red, false, 1.0f);
	return false;
}
