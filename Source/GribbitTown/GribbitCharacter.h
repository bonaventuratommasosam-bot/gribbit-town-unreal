#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GribbitCharacter.generated.h"

class UGribbitNeedsComponent;
class UGribbitOutfitComponent;
class UGribbitInteractionComponent;
class UGribbitBuildingComponent;

USTRUCT(BlueprintType)
struct FGribbitCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefaultOutfit = "Default";
};

UCLASS()
class GRIBBITTOWN_API AGribbitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGribbitCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Apply data from a character preset (for the 7 iconic Gribbits)
	UFUNCTION(BlueprintCallable, Category = "Gribbit")
	void ApplyCharacterData(const FGribbitCharacterData& Data);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<UGribbitNeedsComponent> NeedsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<UGribbitOutfitComponent> OutfitComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<UGribbitInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<UGribbitBuildingComponent> BuildingComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	TObjectPtr<USkeletalMeshComponent> FrogMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TryBuild();
	void TryRemove();
	void TryInteract();
};