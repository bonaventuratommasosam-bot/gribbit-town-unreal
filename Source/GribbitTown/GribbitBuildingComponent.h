#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GribbitBuildingComponent.generated.h"

UENUM(BlueprintType)
enum class EBuildObjectType : uint8
{
	Cube       UMETA(DisplayName = "Cube"),
	Cylinder   UMETA(DisplayName = "Cylinder"),
	Sphere     UMETA(DisplayName = "Sphere"),
	Cone       UMETA(DisplayName = "Cone")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitBuildingComponent();

	UFUNCTION(BlueprintCallable, Category = "Building")
	void PlaceObjectInFront(EBuildObjectType ObjectType = EBuildObjectType::Cube, float Distance = 200.f);

	UFUNCTION(BlueprintCallable, Category = "Building")
	void RemoveObjectInFront(float Distance = 250.f);

	// Change the type of object to build next
	UFUNCTION(BlueprintCallable, Category = "Building")
	void SetBuildType(EBuildObjectType NewType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	EBuildObjectType CurrentBuildType = EBuildObjectType::Cube;
};