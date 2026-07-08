#include "GribbitBuildingComponent.h"
#include "Engine/World.h"

UGribbitBuildingComponent::UGribbitBuildingComponent()
{
}

bool UGribbitBuildingComponent::TryPlaceObject(TSubclassOf<AActor> ObjectClass, const FTransform& Transform)
{
	if (!GetOwner() || !ObjectClass) return false;

	UWorld* World = GetWorld();
	if (!World) return false;

	AActor* NewObject = World->SpawnActor<AActor>(ObjectClass, Transform);
	return NewObject != nullptr;
}
