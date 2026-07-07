"""
build_level.py  —  Gribbits Town (run INSIDE the Unreal Editor)

Feature 3: replaces the old procedural generation with a hand-authored level.
Builds /Game/Maps/GribbitTown_Main.umap: a ground plane, a grid of streets,
lots for each district, the Pizzeria (with a pizza-oven interactable), the
Harbor, and spawn points for the townsfolk.

Run AFTER the C++ module is compiled and the editor is open.
"""

import unreal

MAP_PATH = "/Game/Maps/GribbitTown_Main"

# District anchors (world X,Y). Streets run between them.
DISTRICTS = {
    "TownCenter": (0.0, 0.0),
    "MARFA": (3000.0, 0.0),
    "Pizzeria": (-2500.0, 1500.0),
    "Harbor": (0.0, -3500.0),
}

STREET_MAT = None  # assign a material in the editor if desired


def spawn_actor(cls_path, loc, name):
    world = unreal.EditorLevelLibrary.get_editor_world()
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.load_object(None, cls_path), unreal.Vector(*loc))
    if actor:
        actor.set_actor_label(name)
    return actor


def build_map():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        unreal.EditorLevelLibrary.new_level(MAP_PATH)
    else:
        unreal.EditorLevelLibrary.load_level(MAP_PATH)

    # Ground plane (20000 x 20000, centered).
    spawn_actor("/Script/Engine.StaticMeshActor",
                (0.0, 0.0, 0.0), "Ground")
    # Streets: long thin boxes connecting TownCenter to each district.
    for dist, (x, y) in DISTRICTS.items():
        if dist == "TownCenter":
            continue
        mid = ((x) / 2.0, (y) / 2.0, 1.0)
        spawn_actor("/Script/Engine.StaticMeshActor", mid, f"Street_{dist}")

    # Pizzeria block (interactable handled by BP placed in editor).
    spawn_actor("/Script/Engine.StaticMeshActor",
                (-2500.0, 1500.0, 0.0), "Building_Pizzeria")

    # Harbor water plane.
    spawn_actor("/Script/Engine.StaticMeshActor",
                (0.0, -3500.0, -10.0), "Harbor_Water")

    # Townfolk spawn points (used by GameMode.SpawnTownfolk via DT_Characters).
    for i in range(7):
        sx = 200.0 * (i % 3)
        sy = 200.0 * (i // 3)
        spawn_actor("/Script/Engine.Actor", (sx, sy, 100.0), f"SpawnPoint_{i}")

    unreal.EditorLevelLibrary.save_current_level()
    unreal.log(f"Built {MAP_PATH}. Open it and dress the buildings with real meshes.")


if __name__ == "__main__":
    build_level()
