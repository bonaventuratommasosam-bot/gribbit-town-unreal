#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GribbitVehicle.generated.h"

class AGribbitCharacter;

/**
 * AGribbitVehicle
 *
 * Arcade car. Deliberately NOT a Chaos vehicle: the Kenney car models are
 * plain static meshes with no wheel bones, and a full suspension rig would
 * buy us nothing for a top-of-the-town driving game. This drives on a swept
 * capsule-free model: speed along the forward vector, yaw proportional to
 * steering and speed, a downward trace to stay glued to the road.
 */
UCLASS()
class GRIBBITTOWN_API AGribbitVehicle : public APawn
{
	GENERATED_BODY()

public:
	AGribbitVehicle();

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Puts the frog in the driving seat: hides it and possesses this pawn. */
	UFUNCTION(BlueprintCallable, Category = "Gribbit|Vehicle")
	bool EnterVehicle(AGribbitCharacter* Frog);

	/** Drops the driver back onto the kerb. */
	UFUNCTION(BlueprintCallable, Category = "Gribbit|Vehicle")
	void ExitVehicle();

	UFUNCTION(BlueprintPure, Category = "Gribbit|Vehicle")
	float GetSpeedKmh() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit|Vehicle")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit|Vehicle")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit|Vehicle")
	TObjectPtr<UCameraComponent> Camera;

	/** Top speed forward, cm/s. 2200 is roughly 79 km/h. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gribbit|Guida")
	float MaxSpeed = 2200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gribbit|Guida")
	float MaxReverseSpeed = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gribbit|Guida")
	float Acceleration = 1800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gribbit|Guida")
	float BrakeStrength = 3200.f;

	/** How fast it coasts down with no throttle, as a fraction per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gribbit|Guida")
	float Drag = 0.9f;

	/** Degrees per second at full lock, at speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gribbit|Guida")
	float TurnRate = 95.f;

	/** Below this speed the car barely turns, like a real one. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gribbit|Guida")
	float TurnGripSpeed = 450.f;

	/** How far above the road surface the body sits. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gribbit|Guida")
	float RideHeight = 20.f;

	UPROPERTY(BlueprintReadOnly, Category = "Gribbit|Vehicle")
	TObjectPtr<AGribbitCharacter> Driver;

protected:
	void Throttle(float Value);
	void Steer(float Value);
	void RequestExit();

	void StickToGround(float DeltaSeconds);

	float ThrottleInput = 0.f;
	float SteerInput = 0.f;
	float Speed = 0.f;
};
