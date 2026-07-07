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

- **AGribbitTownGameMode**: set `DefaultFrogClass = BP_ChillPete`,
  `DefaultFrogController = AGribbitPlayerController`,
  `DefaultFrogState = AGribbitPlayerState`, `DefaultMap = /Game/Maps/GribbitTown_Main`.
- **AGribbitPlayerController**: set `NeedsWidgetClass = WBP_NeedsBars`.
- **WBP_NeedsBars**: in the designer, add 6 `UProgressBar`
  (`Bar_Hunger`, `Bar_Energy`, `Bar_Fun`, `Bar_Social`, `Bar_Hygiene`, `Bar_Bladder`)
  and bind `OnNeedsUpdated` -> `SetPercent` on each.

## 4. Test

- PIE (Play In Editor): the frog should appear in GribbitTown_Main, the needs
  HUD should tick down, and looking at an interactable should show a prompt.
- **Multiplayer**: from the editor, `Play > Number of Players = 2` (or "Run as
  Listen Server"). Each client gets its own `AGribbitPlayerState` + HUD.

## 5. Commit

The Python scripts and all C++ are versioned. The generated `.uasset`/`.umap`
are NOT committed (binary); regenerate them from the scripts on any fresh clone.
