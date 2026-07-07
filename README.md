# Gribbit Town - Unreal Edition

Low-poly 3D closed world for the **GRiBBiTS** (frog inhabitants of the Hermes ecosystem).

## References
AI-generated frog concepts in `/References/` — use as modeling + style reference.
Visual style mirrors the Three.js prototype in `C:\Users\pc\gribbit-town`.

## 📦 What's in the repo (ready to compile)

- `GribbitTown.uproject` — UE 5.5 project (C++ module declared)
- `Source/GribbitTown/` — **compiling C++ skeleton**:
  - `GribbitTown.Build.cs` — module deps (Core, Engine, AIModule, NavigationSystem)
  - `GribbitTown.h/.cpp` — primary game module
  - `GribbitTownGameMode.h/.cpp` — default game mode. Spawns the frog pawn and
    **procedurally builds the whole town at StartPlay** (lighting, ground, central
    fountain, 6 buildings, 8 trees, 10 frog NPCs, closed-world perimeter walls).
  - `GribbitCharacter.h/.cpp` — player-controllable frog (WASD, orient-to-movement,
    follow camera on spring arm, skeletal mesh slot for the imported frog model)
- `Config/`
  - `DefaultEngine.ini` — Nanite + Virtual Shadow Maps on, auto-exposure off
  - `DefaultInput.ini` — WASD / arrow axis mappings
  - `DefaultGame.ini` — GlobalDefaultGameMode → GribbitTownGameMode
- `Content/Python/CreateGribbitMap.py` — headless script that creates + saves the map and assigns the GameMode
- `Content/README.md` — suggested content folder layout
- `AVVIA-GRIBBITTOWN.bat` — double-click to launch the editor
- `CREA-MAPPA.bat` — double-click to generate the empty map headlessly (compiles the module + saves `Content/Maps/GribbitTown.umap`)

## ▶️ How to run (3 steps)

### 1. Generate the map (one-time)
Double-click **`CREA-MAPPA.bat`**.
This launches the editor headlessly: it compiles the C++ module, creates an empty
`Content/Maps/GribbitTown.umap`, assigns `GribbitTownGameMode` to it, and saves.
Wait until the window closes (watch the log for "GribbitTown map saved").

### 2. Open the editor
Double-click **`AVVIA-GRIBBITTOWN.bat`** (or the `.uproject`).
The default map opens with the town already built at play-time.

### 3. Play
Press **Play** (or Alt+P). You control the frog with **WASD / arrow keys**,
the follow camera tracks you, and the closed perimeter keeps you in town.

```
"C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe" "C:\Users\pc\Unreal Projects\GribbitTown\GribbitTown.uproject"
```

## 🛠️ What YOU build next (editor-side, optional polish)

1. **Frog character mesh**
   - Import a low-poly frog SkeletalMesh (Blender or Marketplace)
   - Assign it to `AGribbitCharacter` (or make a Blueprint child `BP_Gribbit`)
   - Idle + walk animations, blend by speed
2. **Building detail**
   - The 6 buildings are colored cubes at runtime — replace with real low-poly meshes later
   - Central fountain + trees are runtime shapes too
3. **NPC Gribbits AI**
   - Today the 10 frogs are static green spheres — add spline/idle AI later
4. **Lighting / day-night**
   - Directional light + sky are spawned at runtime; a day/night cycle can come via Blueprint/Timeline

## Notes
- The town is built entirely in C++ at `StartPlay`, so the `.umap` can stay empty.
- C++ skeleton compiles clean; gameplay content can be editor-only from here.
- Disk: ~46 GB free on C: — enough for a local DDC + first build.
