#include "GribbitBuildingComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

UGribbitBuildingComponent::UGribbitBuildingComponent()
{
	SetIsReplicated(true);

	if (!DefaultPlaceableObject)
	{
		DefaultPlaceableObject = AStaticMeshActor::StaticClass();
	}
}

void UGribbitBuildingComponent::PlaceObjectInFront(TSubclassOf<AActor> ObjectClass, float Distance)
{
	if (!ObjectClass) return;

	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	FVector Start = OwnerChar->GetActorLocation();
	FVector Forward = OwnerChar->GetActorForwardVector();
	FVector Location = Start + Forward * Distance;

	FRotator Rotation = OwnerChar->GetActorRotation();
	FRotator FinalRotation(0.f, Rotation.Yaw, 0.f);

	ServerPlaceObject(ObjectClass, Location, FinalRotation);
}

void UGribbitBuildingComponent::RemoveObjectInFront(float Distance)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	FVector Start = OwnerChar->GetActorLocation();
	FVector Forward = OwnerChar->GetActorForwardVector();
	FVector EndLocation = Start + Forward * Distance;

	ServerRemoveObject(EndLocation, Distance);
}

void UGribbitBuildingComponent::ServerPlaceObject_Implementation(TSubclassOf<AActor> ObjectClass, FVector Location, FRotator Rotation)
{
	if (!ObjectClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	AActor* NewActor = World->SpawnActor<AActor>(ObjectClass, Location, Rotation);

	if (AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(NewActor))
	{
		UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
		if (CubeMesh)
		{
			MeshActor->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
		}
	}
}

void UGribbitBuildingComponent::ServerRemoveObject_Implementation(FVector Location, float Distance)
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Location;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->IsA(AStaticMeshActor::StaticClass()))
		{
			HitActor->Destroy();
		}
	}
}
