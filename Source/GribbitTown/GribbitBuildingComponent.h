#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitBuildingComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitBuildingComponent();

	UFUNCTION(BlueprintCallable, Category = "Building")
	void PlaceObjectInFront(TSubclassOf<AActor> ObjectClass, float Distance = 200.f);

	UFUNCTION(BlueprintCallable, Category = "Building")
	void RemoveObjectInFront(float Distance = 250.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TSubclassOf<AActor> DefaultPlaceableObject;

protected:
	UFUNCTION(Server, Reliable)
	void ServerPlaceObject(TSubclassOf<AActor> ObjectClass, FVector Location, FRotator Rotation);

	UFUNCTION(Server, Reliable)
	void ServerRemoveObject(FVector Location, float Distance);
};