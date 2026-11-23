"""
setup_vehicles.py  --  Gribbits Town drivable cars (run INSIDE the Unreal Editor)

    Tools > Execute Python Script  ->  setup_vehicles.py

Requires:
  * the C++ module compiled (AGribbitVehicle must exist), and
  * the Kenney models imported into /Game/Kenney/Vehicles (import_kenney.py).

What it does:
  1. Creates one Blueprint per car model under /Game/Vehicles, each a child of
     AGribbitVehicle, with the Body mesh set and scaled so the car is about
     4.1 m long (a Kenney car imports at roughly 2.55 m).
  2. In the city map, turns the parked scenery cars nearest the player start
     into real drivable ones: same spot, same angle, the static mesh deleted.
  3. Makes sure the map's World Settings use AGribbitTownGameMode, otherwise
     play-in-editor uses the engine default and no frog ever spawns.

In game: walk up to a car and press F to get in, WASD to drive, E to get out.
Every step logs before it runs, so if the editor dies the log says where.
"""

import unreal

BP_DIR = "/Game/Vehicles"
VEHICLE_PARENT = "/Script/GribbitTown.GribbitVehicle"
GAMEMODE = "/Script/GribbitTown.GribbitTownGameMode"
MESH_DIR = "/Game/Kenney/Vehicles"
LEVEL_PATH = "/Game/Maps/GribbitCity"

CAR_M = 4.1              # how long we want a car to be, in metres
WHEEL_CLEARANCE = 10.0   # cm between the road and the mesh pivot
HOW_MANY_DRIVABLE = 5    # scenery cars to promote, nearest the player start

# model in /Game/Kenney/Vehicles -> (blueprint suffix, top speed cm/s)
CARS = [
    ("sedan",         "Sedan",   2200.0),
    ("taxi",          "Taxi",    2100.0),
    ("suv",           "Suv",     2000.0),
    ("police",        "Police",  2600.0),
    ("van",           "Van",     1800.0),
    ("sedan-sports",  "Sports",  2900.0),
]

EAL = unreal.EditorAssetLibrary
ATH = unreal.AssetToolsHelpers.get_asset_tools()


def step(msg):
    unreal.log_warning(">> %s" % msg)


def ensure_dir(path):
    if not EAL.does_directory_exist(path):
        EAL.make_directory(path)


def mesh_length(mesh):
    """Longest horizontal side of the mesh at scale 1, in Unreal units."""
    extent = mesh.get_bounds().box_extent
    return max(extent.x, extent.y) * 2.0


def bp_path_for(suffix):
    return "%s/BP_Car_%s" % (BP_DIR, suffix)


# ------------------------------------------------------------- blueprints

def create_car_bp(model, suffix, max_speed):
    mesh_path = "%s/%s" % (MESH_DIR, model)
    if not EAL.does_asset_exist(mesh_path):
        unreal.log_error("modello mancante: %s -- esegui prima import_kenney.py" % mesh_path)
        return None
    mesh = EAL.load_asset(mesh_path)
    if not isinstance(mesh, unreal.StaticMesh):
        unreal.log_error("non e' una StaticMesh: %s" % mesh_path)
        return None

    path = bp_path_for(suffix)
    if EAL.does_asset_exist(path):
        bp = unreal.load_object(None, path)
    else:
        factory = unreal.BlueprintFactory()
        factory.set_editor_property("parent_class", unreal.load_class(None, VEHICLE_PARENT))
        bp = ATH.create_asset("BP_Car_%s" % suffix, BP_DIR, unreal.Blueprint, factory)
    if not bp:
        unreal.log_error("creazione Blueprint fallita: %s" % path)
        return None

    scale = (CAR_M * 100.0) / max(mesh_length(mesh), 1.0)

    cdo = unreal.get_default_object(bp.generated_class())
    body = cdo.get_editor_property("Body")
    body.set_editor_property("static_mesh", mesh)
    body.set_editor_property("relative_scale3d", unreal.Vector(scale, scale, scale))
    cdo.set_editor_property("MaxSpeed", max_speed)
    # The Kenney pivot sits on the road under the car, so it wants almost no
    # ride height -- just enough to clear the road tile.
    cdo.set_editor_property("RideHeight", WHEEL_CLEARANCE)

    EAL.save_asset(path)
    unreal.log_warning("BP %s  mesh=%s  scala=%.2f  (%.1f m)"
                       % (path, model, scale, mesh_length(mesh) * scale / 100.0))
    return path


# ------------------------------------------------------------------ level

def player_start_location(all_actors):
    """The city's own start if it is labelled, else any PlayerStart."""
    fallback = None
    for a in all_actors:
        if isinstance(a, unreal.PlayerStart):
            if a.get_actor_label().startswith("PuntoDiPartenza"):
                return a.get_actor_location()
            if fallback is None:
                fallback = a.get_actor_location()
    return fallback if fallback else unreal.Vector(0.0, 0.0, 100.0)


def promote_scenery_cars():
    levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    if not EAL.does_asset_exist(LEVEL_PATH):
        unreal.log_error("mappa mancante: %s -- esegui prima build_city.py" % LEVEL_PATH)
        return 0

    step("carico %s" % LEVEL_PATH)
    levels.load_level(LEVEL_PATH)

    step("leggo gli attori del livello")
    all_actors = actors.get_all_level_actors()
    unreal.log_warning("attori nel livello: %d" % len(all_actors))

    step("cerco il punto di partenza")
    origin = player_start_location(all_actors)
    unreal.log_warning("player start: (%.0f, %.0f, %.0f)" % (origin.x, origin.y, origin.z))

    step("World Settings -> GameMode Override")
    try:
        world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
        ws = world.get_world_settings()
        ws.set_editor_property("default_game_mode", unreal.load_class(None, GAMEMODE))
        unreal.log_warning("GameMode override = %s" % GAMEMODE)
    except Exception as exc:
        unreal.log_error("GameMode override non impostato: %s" % exc)

    # The scenery cars from build_city.py are StaticMeshActors labelled Auto_###.
    step("raccolgo le auto di arredo")
    scenery = []
    for a in all_actors:
        if isinstance(a, unreal.StaticMeshActor) and a.get_actor_label().startswith("Auto_"):
            loc = a.get_actor_location()
            d = (loc.x - origin.x) ** 2 + (loc.y - origin.y) ** 2
            scenery.append((d, a))
    scenery.sort(key=lambda pair: pair[0])
    unreal.log_warning("auto di arredo trovate: %d" % len(scenery))
    if not scenery:
        unreal.log_error("nessuna 'Auto_###' nel livello -- esegui prima build_city.py")
        return 0

    chosen = scenery[:HOW_MANY_DRIVABLE]
    spawned = 0
    for i, (_, old) in enumerate(chosen):
        model, suffix, _ = CARS[i % len(CARS)]
        path = bp_path_for(suffix)
        if not EAL.does_asset_exist(path):
            unreal.log_error("Blueprint mancante: %s" % path)
            continue

        loc = old.get_actor_location()
        rot = old.get_actor_rotation()
        label = "AutoGuidabile_%s" % suffix

        step("carico la classe %s" % path)
        name = path.split("/")[-1]
        car_class = unreal.load_object(None, "%s.%s_C" % (path, name))
        if not car_class:
            unreal.log_error("classe non caricata: %s" % path)
            continue

        step("sostituisco %s con %s" % (old.get_actor_label(), label))
        car = actors.spawn_actor_from_class(
            car_class, unreal.Vector(loc.x, loc.y, loc.z + WHEEL_CLEARANCE))
        if not car:
            unreal.log_error("spawn fallito: %s" % label)
            continue

        step("oriento e rinomino %s" % label)
        car.set_actor_rotation(rot, False)
        car.set_actor_label(label)
        actors.destroy_actor(old)
        spawned += 1

    step("salvo il livello")
    levels.save_current_level()
    unreal.log_warning("auto guidabili in %s: %d" % (LEVEL_PATH, spawned))
    return spawned


def main():
    ensure_dir(BP_DIR)
    made = 0
    for model, suffix, max_speed in CARS:
        if create_car_bp(model, suffix, max_speed):
            made += 1
    unreal.log_warning("Blueprint auto create/aggiornate: %d/%d" % (made, len(CARS)))
    promote_scenery_cars()
    unreal.log_warning("setup_vehicles.py completato. In gioco: F per salire, WASD, E per scendere.")


if __name__ == "__main__":
    main()
