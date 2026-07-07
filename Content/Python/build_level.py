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
CUBE = "/Engine/BasicShapes/Cube.Cube"


def spawn_actor(cls_path, loc, name):
    world = unreal.EditorLevelLibrary.get_editor_world()
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.load_object(None, cls_path), unreal.Vector(*loc))
    if actor:
        actor.set_actor_label(name)
    return actor


def spawn_box(name, loc, scale):
    actor = spawn_actor("/Script/Engine.StaticMeshActor", loc, name)
    mesh = unreal.load_object(None, CUBE)
    if actor and mesh:
        actor.static_mesh_component.set_static_mesh(mesh)
        actor.set_actor_scale3d(unreal.Vector(*scale))
    return actor


def spawn_consumable(loc, name, need_amounts, consume=False, prompt="Use"):
    """Place an AGribbitConsumable in the level (Feature 4)."""
    actor = spawn_actor("/Script/GribbitTown.GribbitConsumable", loc, name)
    if actor:
        actor.set_editor_property("need_amounts", need_amounts)
        actor.set_editor_property("b_consume_on_use", consume)
        actor.set_editor_property("interaction_prompt", unreal.Text(prompt))
    return actor


def build_map():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        unreal.EditorLevelLibrary.new_level(MAP_PATH)
    else:
        unreal.EditorLevelLibrary.load_level(MAP_PATH)

    spawn_actor("/Script/Engine.PlayerStart", (0.0, -400.0, 120.0), "PlayerStart_Main")
    spawn_actor("/Script/Engine.DirectionalLight", (0.0, 0.0, 800.0), "Sun")
    spawn_actor("/Script/Engine.SkyLight", (0.0, 0.0, 500.0), "SkyLight")

    # Ground plane (20000 x 20000, centered). Cube default is 100 uu.
    spawn_box("Ground", (0.0, 0.0, -55.0), (200.0, 200.0, 1.0))
    # Streets: long thin boxes connecting TownCenter to each district.
    for dist, (x, y) in DISTRICTS.items():
        if dist == "TownCenter":
            continue
        mid = ((x) / 2.0, (y) / 2.0, 2.0)
        length = max(abs(x), abs(y)) / 100.0
        if abs(x) >= abs(y):
            spawn_box(f"Street_{dist}", mid, (length, 2.2, 0.08))
        else:
            spawn_box(f"Street_{dist}", mid, (2.2, length, 0.08))

    # Pizzeria block.
    spawn_box("Building_Pizzeria", (-2500.0, 1500.0, 140.0), (7.0, 6.0, 3.0))
    # Interactable pizza oven: restores Hunger when used.
    spawn_consumable((-2200.0, 1500.0, 90.0), "PizzaOven",
                     {"Hunger": 45.0, "Fun": 5.0}, consume=False, prompt="Grab a slice")

    # Town-center bench: restores Energy + Fun (a place to chill).
    spawn_box("Bench_TownCenter", (450.0, 180.0, 45.0), (2.4, 0.6, 0.4))
    spawn_consumable((450.0, 220.0, 45.0), "Bench_Rest",
                     {"Energy": 30.0, "Fun": 15.0, "Social": 10.0}, consume=False, prompt="Sit down")

    # Harbor water plane.
    spawn_box("Harbor_Water", (0.0, -3500.0, -40.0), (45.0, 22.0, 0.2))
    spawn_box("Harbor_Dock", (0.0, -2800.0, 10.0), (16.0, 2.0, 0.25))

    # Townfolk spawn points (used by GameMode.SpawnTownfolk via DT_Characters).
    for i in range(7):
        sx = 200.0 * (i % 3)
        sy = 200.0 * (i // 3)
        spawn_actor("/Script/Engine.Actor", (sx, sy, 100.0), f"SpawnPoint_{i}")

    unreal.EditorLevelLibrary.save_current_level()
    unreal.log(f"Built {MAP_PATH}. Open it and dress the buildings with real meshes.")


if __name__ == "__main__":
    build_map()
