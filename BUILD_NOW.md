# Build & run (after toolchain is installed)

## 1. Build the C++ module

From `C:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles`:

```
Build.bat GribbitTownEditor Win64 Development -Project="D:\GribbitTown\GribbitTown.uproject"
```

On this machine (8 GB of RAM) the Unreal Build Accelerator stalls on
"memory pressure" and never spawns a compiler. Add `-NoUBA -MaxParallelActions=1`
and it builds in about three minutes:

```
Build.bat GribbitTownEditor Win64 Development -Project="D:\GribbitTown\GribbitTown.uproject" -NoUBA -MaxParallelActions=1
```

Or open `GribbitTown.uproject` in the editor (it will prompt to build).

## 2. Generate the assets (run once, inside the editor)

Open the editor, then `Outputs > Python > Execute File`:

1. `Content/Python/setup_characters.py`  -> creates BP_ChillPete..BP_LanaMARFA
   and `/Game/Data/DT_Characters`.
2. `Content/Python/setup_needs_widget.py` -> creates `/Game/UI/WBP_NeedsBars`.
3. `Content/Python/build_level.py`       -> creates `/Game/Maps/GribbitTown_Main`.
4. `Content/Python/import_kenney.py`     -> imports the Kenney models from `Art/`.
5. `Content/Python/setup_vehicles.py`    -> creates `/Game/Vehicles/BP_Car_*`
   (the drivable car Blueprints) and sets the map's GameMode override.
   It also tries to park cars in the map; that part crashes the *headless*
   editor on this machine, which is harmless -- the GameMode places the
   drivable cars itself at BeginPlay, so you can skip it.

## 3. Wire up the classes

- **Map GameMode Override** (IMPORTANT — without this PIE uses the engine
  default and you get NO frogs + NO HUD): open `GribbitTown_Main`, then
  **World Settings → GameMode Override = GribbitTownGameMode**, save (Ctrl+S).
  (`GlobalDefaultGameMode` in `Config/DefaultGame.ini` is the fallback, but the
  map currently has its own override set, so this one click is required.)
- **AGribbitTownGameMode**: set `DefaultFrogClass = BP_ChillPete`,
  `DefaultFrogController = AGribbitPlayerController`,
  `DefaultFrogState = AGribbitPlayerState`, `DefaultMap = /Game/Maps/GribbitTown_Main`.
- **AGribbitPlayerController**: set `NeedsWidgetClass = WBP_NeedsBars`.
- **WBP_NeedsBars**: auto-creates its 6 `UProgressBar` bars at runtime, so no
  manual Designer wiring is needed (verified). If you want custom styling, open
  it and add 6 bars named `Bar_Hunger`..`Bar_Bladder` and bind
  `OnNeedsUpdated` -> `SetPercent`.

## 4. Test

- PIE (Play In Editor): a frog spawns in GribbitTown_Main. The needs HUD (6 bars)
  ticks down. Walk up to the **PizzaOven** (near Pizzeria) or **Bench_Rest**
  (town center) and look at it — an interaction prompt appears; press the Interact
  key to restore needs (Hunger from the oven, Energy/Fun/Social from the bench).
- **Cars**: the five parked cars nearest the player start are swapped for
  drivable ones when play begins (`AGribbitTownGameMode::SpawnDrivableCars`,
  logged as "Gribbit: N parked cars are now drivable"). Walk up to one and
  press **F** to get in, **WASD** to drive, **E** to get out. The boarding
  range is `AGribbitPlayerController.VehicleBoardingRange` (400 cm).
- **Multiplayer**: from the editor, `Play > Number of Players = 2` (or "Run as
  Listen Server"). Each client gets its own `AGribbitPlayerState` + HUD.

## 5. Commit

The Python scripts and all C++ are versioned. The generated `.uasset`/`.umap`
are NOT committed (binary); regenerate them from the scripts on any fresh clone.
