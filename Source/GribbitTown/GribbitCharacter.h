#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GribbitCharacter.generated.h"

/**
 * The player-controllable Gribbit (frog).
 * Low-poly character with a skeletal mesh slot, ready to receive
 * the imported frog model + idle/walk animations, plus a follow camera.
 */
UCLASS()
class GRIBBITTOWN_API AGribbitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGribbitCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// Skeletal mesh placeholder for the frog (assign in Blueprint / editor).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit")
	TObjectPtr<USkeletalMeshComponent> FrogMesh;

	// Spring arm holding the follow camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit")
	TObjectPtr<USpringArmComponent> SpringArm;

	// Player follow camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit")
	TObjectPtr<UCameraComponent> FollowCamera;

	void MoveForward(float Value);
	void MoveRight(float Value);
};
