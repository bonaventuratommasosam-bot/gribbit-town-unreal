"""
Build Gribbit City: a road grid with blocks of buildings and parked cars.

Runs entirely in the editor, no C++ build required.
    Tools > Execute Python Script > build_city.py

It measures the real bounds of an imported road tile instead of assuming a
scale, so the grid lines up whatever units the FBX came in with.
"""

import math
import random
import unreal

SEED = 11
BLOCKS_X, BLOCKS_Y = 6, 5      # city blocks
BLOCK_TILES = 4                # road tiles along one side of a block
LEVEL_PATH = "/Game/Maps/GribbitCity"

EAL = unreal.EditorAssetLibrary
actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)

rnd = random.Random(SEED)


# ----------------------------------------------------------------- helpers

def load_mesh(path):
    return EAL.load_asset(path) if EAL.does_asset_exist(path) else None


def meshes_in(folder):
    out = {}
    if not EAL.does_directory_exist(folder):
        return out
    for p in EAL.list_assets(folder, recursive=False, include_folder=False):
        a = EAL.load_asset(p)
        if isinstance(a, unreal.StaticMesh):
            out[p.split("/")[-1].split(".")[0]] = a
    return out


def mesh_size(mesh):
    """World-space size of a static mesh at scale 1, in Unreal units."""
    b = mesh.get_bounds().box_extent
    return (b.x * 2.0, b.y * 2.0, b.z * 2.0)


def place(mesh, x, y, z=0.0, yaw=0.0, scale=1.0, label=None, folder=None):
    a = actors.spawn_actor_from_class(
        unreal.StaticMeshActor,
        unreal.Vector(x, y, z),
        unreal.Rotator(0.0, 0.0, yaw),
    )
    a.static_mesh_component.set_static_mesh(mesh)
    a.set_actor_scale3d(unreal.Vector(scale, scale, scale))
    a.set_mobility(unreal.ComponentMobility.STATIC)
    if label:
        a.set_actor_label(label)
    if folder:
        a.set_folder_path(folder)
    return a


# -------------------------------------------------------------------- city

def build():
    roads = meshes_in("/Game/Kenney/Roads")
    if not roads:
        unreal.log_error("Nessun modello in /Game/Kenney/Roads — esegui prima import_kenney.py")
        return

    straight = roads.get("road-straight")
    cross = roads.get("road-crossroad") or roads.get("road-crossroad-line")
    bend = roads.get("road-bend")
    if not straight or not cross:
        unreal.log_error("Mancano road-straight o road-crossroad fra i modelli importati")
        return

    # one road tile, measured not guessed
    tw, th, _ = mesh_size(straight)
    tile = max(tw, th)
    # A Kenney tile imports as 1 m. A two-lane street wants about 8 m across,
    # and a car is 2.55 m long at scale 1, so it wants about 1.6x to reach 4 m.
    ROAD_M = 8.0
    CAR_M = 4.1
    road_scale = (ROAD_M * 100.0) / tile
    step = tile * road_scale
    unreal.log_warning("piastrella: %.0f uu -> scala %.2f, passo %.0f uu (%.1f m)"
                       % (tile, road_scale, step, step / 100.0))

    buildings = {}
    for cat in ("Commercial", "Industrial", "Suburban"):
        for name, m in meshes_in("/Game/Kenney/%s" % cat).items():
            if name.startswith(("building", "skyscraper", "house", "shop", "industry", "low")):
                buildings[name] = m
    cars = {n: m for n, m in meshes_in("/Game/Kenney/Vehicles").items()
            if not n.startswith(("debris", "cone", "box", "wheel", "tire"))}
    unreal.log_warning("disponibili: %d edifici, %d veicoli" % (len(buildings), len(cars)))

    levels.new_level(LEVEL_PATH)

    span_x = BLOCKS_X * (BLOCK_TILES + 1)
    span_y = BLOCKS_Y * (BLOCK_TILES + 1)

    # ---- roads: a tile is a junction when both indices sit on a grid line
    for ix in range(span_x + 1):
        for iy in range(span_y + 1):
            on_x = ix % (BLOCK_TILES + 1) == 0
            on_y = iy % (BLOCK_TILES + 1) == 0
            if not (on_x or on_y):
                continue
            x, y = ix * step, iy * step
            if on_x and on_y:
                place(cross, x, y, 0, 0, road_scale, "Incrocio_%d_%d" % (ix, iy), "Citta/Strade")
            else:
                yaw = 90.0 if on_y else 0.0
                place(straight, x, y, 0, yaw, road_scale, "Strada_%d_%d" % (ix, iy), "Citta/Strade")

    # ---- blocks: buildings packed inside, facing the street
    names = sorted(buildings)
    for bx in range(BLOCKS_X):
        for by in range(BLOCKS_Y):
            x0 = (bx * (BLOCK_TILES + 1) + 1) * step
            y0 = (by * (BLOCK_TILES + 1) + 1) * step
            if rnd.random() < 0.12:
                continue                       # leave a gap: car park or lot
            for tx in range(BLOCK_TILES):
                for ty in range(BLOCK_TILES):
                    edge = tx in (0, BLOCK_TILES - 1) or ty in (0, BLOCK_TILES - 1)
                    if rnd.random() < (0.12 if edge else 0.45):
                        continue
                    name = rnd.choice(names)
                    if not edge:                       # towers go in the middle
                        tall = [n for n in names if "skyscraper" in n]
                        if tall and rnd.random() < 0.7:
                            name = rnd.choice(tall)
                    m = buildings[name]
                    bw, bd, bht = mesh_size(m)
                    # fit the footprint to the tile, then vary the height a bit
                    fit = (step * 0.92) / max(bw, bd, 1.0)
                    s = fit * (0.85 + rnd.random() * 0.3)
                    if ty == 0:
                        yaw = 180.0
                    elif ty == BLOCK_TILES - 1:
                        yaw = 0.0
                    elif tx == 0:
                        yaw = 90.0
                    else:
                        yaw = 270.0
                    place(m, x0 + tx * step, y0 + ty * step, 0, yaw, s,
                          "Edificio_%d_%d_%d_%d" % (bx, by, tx, ty), "Citta/Edifici")

    # ---- parked cars along the kerbs
    carnames = sorted(cars)
    if carnames:
        sample = cars[carnames[0]]
        _, car_len, _ = mesh_size(sample)
        car_scale = (CAR_M * 100.0) / max(car_len, 1.0)
        unreal.log_warning("auto: %.0f uu -> scala %.2f (%.1f m)"
                           % (car_len, car_scale, car_len * car_scale / 100.0))
        placed = 0
        for _ in range(220):
            if placed >= 70:
                break
            horizontal = rnd.random() < 0.5
            lane = rnd.randrange(span_y if horizontal else span_x)
            lane = lane // (BLOCK_TILES + 1) * (BLOCK_TILES + 1)
            along = rnd.randrange(span_x if horizontal else span_y)
            if along % (BLOCK_TILES + 1) == 0:
                continue                      # keep junctions clear
            off = step * (0.22 if rnd.random() < 0.5 else -0.22)
            if horizontal:
                x, y, yaw = along * step, lane * step + off, 0.0 if off > 0 else 180.0
            else:
                x, y, yaw = lane * step + off, along * step, 90.0 if off > 0 else 270.0
            place(cars[rnd.choice(carnames)], x, y, 0, yaw, car_scale,
                  "Auto_%03d" % placed, "Citta/Auto")
            placed += 1

    # ---- light and sky
    sun = actors.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 1200),
                                        unreal.Rotator(0, -48, 25))
    sun.set_actor_label("Sole")
    sun.light_component.set_intensity(6.0)
    actors.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1400)).set_actor_label("LuceAmbiente")
    actors.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0)).set_actor_label("Atmosfera")
    actors.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0, 0, 100)).set_actor_label("Foschia")

    # ---- somewhere to spawn when we press play
    ps = actors.spawn_actor_from_class(
        unreal.PlayerStart,
        unreal.Vector(step * 1.0, step * 1.0, 120.0),
        unreal.Rotator(0, 0, 45))
    ps.set_actor_label("PuntoDiPartenza")

    # ---- a ground plane under everything
    plane = load_mesh("/Engine/BasicShapes/Plane.Plane")
    if plane:
        pw, _, _ = mesh_size(plane)
        s = (max(span_x, span_y) + 4) * step / pw
        place(plane, span_x * step / 2, span_y * step / 2, -2.0, 0, s, "Terreno", "Citta")

    levels.save_current_level()
    unreal.log_warning("Citta costruita: %d x %d isolati, livello %s" % (BLOCKS_X, BLOCKS_Y, LEVEL_PATH))


if __name__ == "__main__":
    build()
