#include "GribbitBuildingComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

UGribbitBuildingComponent::UGribbitBuildingComponent()
{
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
	return NewActor != nullptr;
}
