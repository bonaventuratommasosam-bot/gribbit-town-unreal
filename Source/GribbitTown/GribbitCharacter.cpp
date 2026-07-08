#include "GribbitCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GribbitNeedsComponent.h"
#include "GribbitOutfitComponent.h"
#include "GribbitInteractionComponent.h"
#include "GribbitBuildingComponent.h"

AGribbitCharacter::AGribbitCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	FrogMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FrogMesh"));
	FrogMesh->SetupAttachment(GetCapsuleComponent());
	FrogMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	NeedsComponent = CreateDefaultSubobject<UGribbitNeedsComponent>(TEXT("NeedsComponent"));
	OutfitComponent = CreateDefaultSubobject<UGribbitOutfitComponent>(TEXT("OutfitComponent"));
	InteractionComponent = CreateDefaultSubobject<UGribbitInteractionComponent>(TEXT("InteractionComponent"));
	BuildingComponent = CreateDefaultSubobject<UGribbitBuildingComponent>(TEXT("BuildingComponent"));
}

void AGribbitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGribbitCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGribbitCharacter::MoveRight);

	// Building input (tasto B per piazzare)
	PlayerInputComponent->BindAction("Build", IE_Pressed, this, &AGribbitCharacter::TryBuild);
}

void AGribbitCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
	}
}

void AGribbitCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
	}
}

void AGribbitCharacter::TryBuild()
{
	if (BuildingComponent && BuildingComponent->DefaultPlaceableObject)
	{
		BuildingComponent->PlaceObjectInFront(BuildingComponent->DefaultPlaceableObject);
	}
}
