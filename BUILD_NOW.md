# Build & run (after toolchain is installed)

## 1. Build the C++ module

From `C:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles`:

```
Build.bat GribbitTownEditor Win64 Development -Project="C:\Users\pc\Unreal Projects\GribbitTown\GribbitTown.uproject"
```

Or open `GribbitTown.uproject` in the editor (it will prompt to build).

## 2. Generate the assets (run once, inside the editor)

Open the editor, then `Outputs > Python > Execute File`:

1. `Content/Python/setup_characters.py`  -> creates BP_ChillPete..BP_LanaMARFA
   and `/Game/Data/DT_Characters`.
2. `Content/Python/setup_needs_widget.py` -> creates `/Game/UI/WBP_NeedsBars`.
3. `Content/Python/build_level.py`       -> creates `/Game/Maps/GribbitTown_Main`.

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
- **Multiplayer**: from the editor, `Play > Number of Players = 2` (or "Run as
  Listen Server"). Each client gets its own `AGribbitPlayerState` + HUD.

## 5. Commit

The Python scripts and all C++ are versioned. The generated `.uasset`/`.umap`
are NOT committed (binary); regenerate them from the scripts on any fresh clone.
