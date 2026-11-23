#include "GribbitVehicle.h"

#include "GribbitCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

AGribbitVehicle::AGribbitVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Body->SetCollisionObjectType(ECC_Pawn);
	Body->SetCollisionResponseToAllChannels(ECR_Block);
	Body->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Body->SetSimulatePhysics(false);
	Body->SetMobility(EComponentMobility::Movable);
	RootComponent = Body;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 650.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 220.f);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 6.f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(-14.f, 0.f, 0.f));

	bUseControllerRotationYaw = false;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void AGribbitVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!PlayerInputComponent)
	{
		return;
	}
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGribbitVehicle::Throttle);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AGribbitVehicle::Steer);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AGribbitVehicle::RequestExit);
}

void AGribbitVehicle::Throttle(float Value)
{
	ThrottleInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AGribbitVehicle::Steer(float Value)
{
	SteerInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AGribbitVehicle::RequestExit()
{
	ExitVehicle();
}

float AGribbitVehicle::GetSpeedKmh() const
{
	return FMath::Abs(Speed) * 0.036f;
}

void AGribbitVehicle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (DeltaSeconds <= 0.f)
	{
		return;
	}

	// throttle, brake, coast
	if (ThrottleInput > KINDA_SMALL_NUMBER)
	{
		Speed += Acceleration * ThrottleInput * DeltaSeconds;
	}
	else if (ThrottleInput < -KINDA_SMALL_NUMBER)
	{
		// pressing back is a brake while rolling forward, reverse once stopped
		const float Push = (Speed > 10.f) ? BrakeStrength : Acceleration;
		Speed += Push * ThrottleInput * DeltaSeconds;
	}
	else
	{
		Speed *= FMath::Clamp(1.f - Drag * DeltaSeconds, 0.f, 1.f);
		if (FMath::Abs(Speed) < 2.f)
		{
			Speed = 0.f;
		}
	}
	Speed = FMath::Clamp(Speed, -MaxReverseSpeed, MaxSpeed);

	// steering bites more the faster you go, and reverses in reverse
	if (!FMath::IsNearlyZero(SteerInput) && FMath::Abs(Speed) > 1.f)
	{
		const float Grip = FMath::Clamp(FMath::Abs(Speed) / TurnGripSpeed, 0.f, 1.f);
		const float Direction = (Speed >= 0.f) ? 1.f : -1.f;
		AddActorWorldRotation(FRotator(0.f, SteerInput * TurnRate * Grip * Direction * DeltaSeconds, 0.f));
	}

	// move, and lose the speed into whatever we hit
	if (!FMath::IsNearlyZero(Speed))
	{
		FHitResult Hit;
		AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaSeconds, true, &Hit);
		if (Hit.bBlockingHit)
		{
			Speed *= -0.18f;
		}
	}

	StickToGround(DeltaSeconds);
}

void AGribbitVehicle::StickToGround(float DeltaSeconds)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Origin = GetActorLocation();
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(GribbitVehicleGround), false, this);
	const bool bHit = World->LineTraceSingleByChannel(
		Hit, Origin + FVector(0, 0, 150.f), Origin - FVector(0, 0, 400.f), ECC_Visibility, Params);

	if (!bHit)
	{
		return;
	}

	const float TargetZ = Hit.ImpactPoint.Z + RideHeight;
	FVector Loc = Origin;
	Loc.Z = FMath::FInterpTo(Origin.Z, TargetZ, DeltaSeconds, 12.f);
	SetActorLocation(Loc, false);

	// lean the body to follow the slope of the road
	const FRotator Current = GetActorRotation();
	const FRotator Slope = FRotationMatrix::MakeFromZX(Hit.ImpactNormal, GetActorForwardVector()).Rotator();
	const FRotator Blended(
		FMath::FInterpTo(Current.Pitch, Slope.Pitch, DeltaSeconds, 8.f),
		Current.Yaw,
		FMath::FInterpTo(Current.Roll, Slope.Roll, DeltaSeconds, 8.f));
	SetActorRotation(Blended);
}

bool AGribbitVehicle::EnterVehicle(AGribbitCharacter* Frog)
{
	if (!Frog || Driver)
	{
		return false;
	}
	AController* C = Frog->GetController();
	if (!C)
	{
		return false;
	}

	Driver = Frog;
	Speed = 0.f;
	ThrottleInput = 0.f;
	SteerInput = 0.f;

	Frog->SetActorEnableCollision(false);
	Frog->SetActorHiddenInGame(true);
	if (UCharacterMovementComponent* Move = Frog->GetCharacterMovement())
	{
		Move->StopMovementImmediately();
		Move->DisableMovement();
	}
	Frog->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	C->Possess(this);
	return true;
}

void AGribbitVehicle::ExitVehicle()
{
	if (!Driver)
	{
		return;
	}

	AGribbitCharacter* Frog = Driver;
	Driver = nullptr;

	AController* C = GetController();

	// step out on the left, clear of the bodywork
	FVector Side = GetActorRightVector() * -180.f;
	FVector Drop = GetActorLocation() + Side + FVector(0, 0, 60.f);

	Frog->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Frog->SetActorLocation(Drop, false);
	Frog->SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
	Frog->SetActorHiddenInGame(false);
	Frog->SetActorEnableCollision(true);
	if (UCharacterMovementComponent* Move = Frog->GetCharacterMovement())
	{
		Move->SetMovementMode(MOVE_Walking);
	}

	Speed = 0.f;
	if (C)
	{
		C->Possess(Frog);
	}
}
