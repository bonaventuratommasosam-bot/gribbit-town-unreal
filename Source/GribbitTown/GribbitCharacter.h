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

/**
 * AGribbitCharacter
 * 
 * Personaggio principale giocabile (Gribbit).
 * 
 * Include:
 * - Movimento
 * - Sistema Bisogni (Needs)
 * - Sistema Outfit
 * - Sistema Interazione
 * - Sistema Costruzione (Building) - stile Mini World
 * 
 * I 7 personaggi iconici saranno versioni specializzate di questa classe.
 */
UCLASS()
class GRIBBITTOWN_API AGribbitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGribbitCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// === Core Systems ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<class UGribbitNeedsComponent> NeedsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<class UGribbitOutfitComponent> OutfitComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<class UGribbitInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gribbit Systems")
	TObjectPtr<class UGribbitBuildingComponent> BuildingComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	TObjectPtr<USkeletalMeshComponent> FrogMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	void MoveForward(float Value);
	void MoveRight(float Value);
};