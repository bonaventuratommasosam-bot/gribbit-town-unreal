"""
Foto della citta, catturate in modo SINCRONO dentro l'editor vero.

  UnrealEditor.exe <project> -ExecutePythonScript="...\\render_shots.py"

L'editor esce da solo a fine script. Le immagini finiscono in <progetto>/Shots.
"""

import os
import unreal

LEVEL = "/Game/Maps/GribbitCity"
OUT_DIR = os.path.join(unreal.Paths.project_dir(), "Shots")
W, H = 1920, 1080

# La citta va da (0,0) a circa (24000, 20000); le strade stanno sui multipli
# di 4000 uu, gli isolati in mezzo.
SHOTS = [
    ("01_aerea",     (-4000, -4000, 17000), (-50,  45, 0), 70),
    ("02_skyline",   (12000, -7000,  2600), ( -7,  90, 0), 65),
    ("03_incrocio",  (10400, 10400,  4200), (-40,  45, 0), 70),
    ("04_strada",    ( 5200,  4000,   260), ( -3,   0, 0), 80),
    ("05_auto",      ( 9400,  8000,   200), ( -4,   8, 0), 75),
    ("06_isolato",   (14000, 12000,  1500), (-22, 200, 0), 70),
]


def run():
    if not unreal.EditorAssetLibrary.does_asset_exist(LEVEL):
        unreal.log_error("Livello mancante: %s" % LEVEL)
        return

    lv = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    ue = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)
    acts = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    lv.load_level(LEVEL)
    world = ue.get_editor_world()
    unreal.log_warning("mondo: %s, attori: %d" % (world.get_name(), len(acts.get_all_level_actors())))

    if not os.path.isdir(OUT_DIR):
        os.makedirs(OUT_DIR)

    rt = unreal.RenderingLibrary.create_render_target2d(
        world, W, H, unreal.TextureRenderTargetFormat.RTF_RGBA8)

    cap = acts.spawn_actor_from_class(unreal.SceneCapture2D,
                                      unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    comp = cap.capture_component2d
    comp.set_editor_property("texture_target", rt)
    comp.set_editor_property("capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_LDR)
    comp.set_editor_property("capture_every_frame", False)
    comp.set_editor_property("capture_on_movement", False)
    comp.set_editor_property("always_persist_rendering_state", True)

    made = 0
    for name, (x, y, z), (pitch, yaw, roll), fov in SHOTS:
        cap.set_actor_location_and_rotation(
            unreal.Vector(float(x), float(y), float(z)),
            unreal.Rotator(float(roll), float(pitch), float(yaw)), False, False)
        comp.set_editor_property("fov_angle", float(fov))
        # due passate: la prima scalda streaming e shader, la seconda e' buona
        comp.capture_scene()
        comp.capture_scene()
        unreal.RenderingLibrary.export_render_target(world, rt, OUT_DIR, name + ".png")
        path = os.path.join(OUT_DIR, name + ".png")
        size = os.path.getsize(path) if os.path.exists(path) else 0
        unreal.log_warning("scatto %-14s %d byte" % (name, size))
        if size > 2000:
            made += 1

    acts.destroy_actor(cap)
    unreal.log_warning("SCATTI FATTI: %d su %d in %s" % (made, len(SHOTS), OUT_DIR))


run()
