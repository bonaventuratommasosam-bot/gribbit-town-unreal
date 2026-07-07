#include "GribbitTownGameMode.h"
#include "GribbitCharacter.h"
#include "GribbitPlayerController.h"
#include "GribbitPlayerState.h"
#include "GribbitCharacterData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
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
}

void AGribbitTownGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Feature 3: load the hand-authored level instead of generating procedurally.
	UWorld* World = GetWorld();
	if (World && !DefaultMap.IsEmpty() && !World->GetMapName().Contains(TEXT("GribbitTown_Main")))
	{
		UGameplayStatics::OpenLevel(World, FName(*DefaultMap));
		return;
	}

	SpawnTownfolk();
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

	const int32 Count = FMath::Min(TownfolkCount, Rows.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		const FName RowName = Table->GetRowNames()[i];
		const FVector Loc(200.f * (i % 3), 200.f * (i / 3), 100.f);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (AGribbitCharacter* Frog = World->SpawnActor<AGribbitCharacter>(DefaultFrogClass, Loc, FRotator::ZeroRotator, Params))
		{
			Frog->ApplyPreset(RowName);
		}
	}
}
