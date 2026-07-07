#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GribbitCharacter.generated.h"

class UGribbitNeedsComponent;
class UGribbitOutfitComponent;
class UGribbitInteractionComponent;

/**
 * AGribbitCharacter
 * 
 * Base player-controlled Gribbit.
 * 
 * Features:
 * - Movement (WASD + orient to movement)
 * - Follow camera on spring arm
 * - GribbitNeedsComponent (Hunger, Energy, Fun, Social...)
 * - GribbitOutfitComponent (for iconic looks: Pizza, Cowboy, MARFA, etc.)
 * 
 * The 7 iconic characters (Chill Pete, Sheriff Buck, Max MARFA...) will be
 * implemented as child Blueprints or subclasses of this.
 */
UCLASS()
class GRIBBITTOWN_API AGribbitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGribbitCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Core Systems
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<UGribbitNeedsComponent> NeedsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<UGribbitOutfitComponent> OutfitComponent;

	// Interaction system (Feature 4)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<UGribbitInteractionComponent> InteractionComponent;

	// Preset key into DT_Characters; set by Blueprint children or the GameMode.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Gribbit")
	FName CharacterPresetID = NAME_None;

	// Applies a DT_Characters row to this character (needs, outfit, tint).
	UFUNCTION(BlueprintCallable, Category = "Gribbit")
	void ApplyPreset(const FName& PresetID);

	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit")
	TObjectPtr<USkeletalMeshComponent> FrogMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit")
	TObjectPtr<UCameraComponent> FollowCamera;

	void MoveForward(float Value);
	void MoveRight(float Value);
};
