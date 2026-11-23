#include "GribbitTownGameMode.h"
#include "GribbitCharacter.h"
#include "GribbitPlayerController.h"
#include "GribbitPlayerState.h"
#include "GribbitCharacterData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "GribbitVehicle.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

AGribbitTownGameMode::AGribbitTownGameMode()
{
	static ConstructorHelpers::FClassFinder<AGribbitCharacter> ChillPeteBP(TEXT("/Game/Characters/BP_ChillPete"));
	if (ChillPeteBP.Succeeded())
	{
		DefaultFrogClass = ChillPeteBP.Class;
	}

	// Multiplayer-ready default classes (Feature 5).
	DefaultPawnClass = DefaultFrogClass ? DefaultFrogClass.Get() : AGribbitCharacter::StaticClass();
	PlayerControllerClass = AGribbitPlayerController::StaticClass();
	PlayerStateClass = AGribbitPlayerState::StaticClass();

	// The chosen frog (BP_ChillPete after the Python setup) is the default pawn.
	if (!DefaultFrogClass)
	{
		DefaultFrogClass = AGribbitCharacter::StaticClass();
	}

	// The drivable cars setup_vehicles.py generates. Missing ones are skipped,
	// so the game still runs on a clone where the script has not been run.
	static const TCHAR* CarBlueprints[] = {
		TEXT("/Game/Vehicles/BP_Car_Sedan"),
		TEXT("/Game/Vehicles/BP_Car_Taxi"),
		TEXT("/Game/Vehicles/BP_Car_Suv"),
		TEXT("/Game/Vehicles/BP_Car_Police"),
		TEXT("/Game/Vehicles/BP_Car_Van"),
		TEXT("/Game/Vehicles/BP_Car_Sports"),
	};
	for (const TCHAR* Path : CarBlueprints)
	{
		ConstructorHelpers::FClassFinder<AGribbitVehicle> Found(Path);
		if (Found.Succeeded())
		{
			DrivableCarClasses.Add(Found.Class);
		}
	}
}

void AGribbitTownGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnTownfolk();
	SpawnDrivableCars();
}

void AGribbitTownGameMode::SpawnTownfolk()
{
	UWorld* World = GetWorld();
	if (!World || !DefaultFrogClass) return;

	// Load the character table and spawn one frog per row.
	UDataTable* Table = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Characters.DT_Characters"));
	if (!Table) return;

	TArray<FGribbitCharacterRow*> Rows;
	Table->GetAllRows<FGribbitCharacterRow>(TEXT("SpawnTownfolk"), Rows);

	// Gather round whatever the level offers as a player start, so the frogs
	// turn up where the player does instead of at the world origin.
	FVector Anchor(0.f, 0.f, 100.f);
	TArray<AActor*> Starts;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), Starts);
	if (Starts.Num() > 0 && Starts[0])
	{
		Anchor = Starts[0]->GetActorLocation();
	}

	const int32 Count = FMath::Min(TownfolkCount, Rows.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		const FName RowName = Table->GetRowNames()[i];

		// A ring around the anchor: close enough to meet, clear of the start.
		const float Angle = (2.f * PI * i) / FMath::Max(Count, 1);
		const FVector Loc = Anchor + FVector(
			FMath::Cos(Angle) * TownfolkSpread,
			FMath::Sin(Angle) * TownfolkSpread,
			0.f);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (AGribbitCharacter* Frog = World->SpawnActor<AGribbitCharacter>(DefaultFrogClass, Loc, FRotator::ZeroRotator, Params))
		{
			Frog->ApplyPreset(RowName);
		}
	}
}

void AGribbitTownGameMode::SpawnDrivableCars()
{
	UWorld* World = GetWorld();
	if (!World || DrivableCarCount <= 0 || DrivableCarClasses.Num() == 0)
	{
		return;
	}

	// Where the player turns up: the cars should be within walking distance.
	FVector Anchor(0.f, 0.f, 0.f);
	TArray<AActor*> Starts;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), Starts);
	if (Starts.Num() > 0 && Starts[0])
	{
		Anchor = Starts[0]->GetActorLocation();
	}

	// A parked scenery car is a StaticMeshActor showing a mesh out of the
	// Kenney vehicle set. Matching on the mesh path keeps this working
	// whatever the actors ended up being called, and actor labels do not
	// exist outside the editor anyway.
	TArray<TPair<float, AStaticMeshActor*>> Candidates;
	const float RadiusSq = DrivableCarSearchRadius * DrivableCarSearchRadius;

	for (TActorIterator<AStaticMeshActor> It(World); It; ++It)
	{
		AStaticMeshActor* Actor = *It;
		if (!Actor)
		{
			continue;
		}
		const UStaticMeshComponent* Comp = Actor->GetStaticMeshComponent();
		const UStaticMesh* Mesh = Comp ? Comp->GetStaticMesh() : nullptr;
		if (!Mesh || !Mesh->GetPathName().Contains(TEXT("/Game/Kenney/Vehicles/")))
		{
			continue;
		}
		const float DistSq = FVector::DistSquared2D(Anchor, Actor->GetActorLocation());
		if (DistSq <= RadiusSq)
		{
			Candidates.Emplace(DistSq, Actor);
		}
	}

	Candidates.Sort([](const TPair<float, AStaticMeshActor*>& A, const TPair<float, AStaticMeshActor*>& B)
	{
		return A.Key < B.Key;
	});

	const int32 Count = FMath::Min(DrivableCarCount, Candidates.Num());
	int32 Swapped = 0;

	for (int32 i = 0; i < Count; ++i)
	{
		AStaticMeshActor* Scenery = Candidates[i].Value;
		TSubclassOf<AGribbitVehicle> CarClass = DrivableCarClasses[i % DrivableCarClasses.Num()];
		if (!Scenery || !CarClass)
		{
			continue;
		}

		const FVector Loc = Scenery->GetActorLocation();
		const FRotator Rot(0.f, Scenery->GetActorRotation().Yaw, 0.f);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AGribbitVehicle* Car = World->SpawnActor<AGribbitVehicle>(CarClass, Loc, Rot, Params))
		{
			// The scenery mesh would sit inside the drivable one.
			Scenery->Destroy();
			++Swapped;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Gribbit: %d parked cars are now drivable (of %d nearby)"),
		Swapped, Candidates.Num());
}
