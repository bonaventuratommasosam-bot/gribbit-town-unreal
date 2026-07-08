#include "GribbitBuildingComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

UGribbitBuildingComponent::UGribbitBuildingComponent()
{
	SetIsReplicated(true);
}

void UGribbitBuildingComponent::PlaceObjectInFront(EBuildObjectType ObjectType, float Distance)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector Start = OwnerChar->GetActorLocation();
	FVector Forward = OwnerChar->GetActorForwardVector();
	FVector Location = Start + Forward * Distance;

	FRotator Rotation = OwnerChar->GetActorRotation();
	FRotator FinalRotation(0.f, Rotation.Yaw, 0.f);

	AActor* NewActor = World->SpawnActor<AActor>(AStaticMeshActor::StaticClass(), Location, FinalRotation);
	AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(NewActor);
	if (!MeshActor) return;

	UStaticMesh* Mesh = nullptr;

	switch (ObjectType)
	{
	case EBuildObjectType::Cube:     Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube")); break;
	case EBuildObjectType::Cylinder: Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder")); break;
	case EBuildObjectType::Sphere:   Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere")); break;
	case EBuildObjectType::Cone:     Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone")); break;
	default: Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube")); break;
	}

	if (Mesh)
	{
		MeshActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	}
}

void UGribbitBuildingComponent::RemoveObjectInFront(float Distance)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	UWorld* World = GetWorld();
	if (!World) return;

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
		}
	}
}

void UGribbitBuildingComponent::SetBuildType(EBuildObjectType NewType)
{
	CurrentBuildType = NewType;
}
