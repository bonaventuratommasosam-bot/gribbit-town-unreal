"""
setup_base_scene.py  --  Gribbit Town (canon project: Documents/design)
B: adds a playable base scene to GribbitTown_Main:
   - ground plane
   - DirectionalLight (sun) with shadows
   - real-time SkyLight (no static build needed -> no 'waiting on shaders')
   - SkyAtmosphere (blue sky for the SkyLight to capture)
   - PlayerStart
A: removes any stuck STATIC SkyLight, then attempts a lighting build.
Saves the level at the end.
"""

import unreal
import traceback

MAP = "/Game/Maps/GribbitTown_Main"
DONE = "C:/Users/pc/Documents/design/setup_base_done.marker"
ERR = "C:/Users/pc/Documents/design/setup_base_error.log"


def main():
    try:
        editor = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
        actor_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

        if not unreal.EditorAssetLibrary.does_asset_exist(MAP):
            raise RuntimeError("Map %s not found" % MAP)
        editor.load_level(MAP)

        # --- A: remove stuck static lights/skylights (cause of black + 'waiting') ---
        for a in actor_sub.get_all_level_actors():
            c = a.get_class()
            cn = c.get_name()
            if cn in ("SkyLight", "DirectionalLight", "SkyAtmosphere", "PlayerStart"):
                actor_sub.destroy_actor(a)

        # --- B: ground plane ---
        ground = actor_sub.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0))
        if ground:
            plane = unreal.load_object(None, "/Engine/BasicShapes/Plane.Plane")
            smc = ground.static_mesh_component
            if plane and smc:
                smc.set_editor_property("static_mesh", plane)
                ground.set_actor_scale3d(unreal.Vector(100, 100, 1))
                ground.set_actor_label("Ground_Town")
                # simple lit material so it's not pure black
                try:
                    mat = unreal.load_object(None, "/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")
                    if mat:
                        smc.set_editor_property("override_materials", [mat])
                except Exception as e:
                    unreal.log_warning("ground mat: %s" % e)

        # --- B: Directional Light (sun) ---
        sun = actor_sub.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 600))
        if sun:
            sun.set_actor_rotation(unreal.Rotator(-55, 45, 0), False)
            sun.set_actor_label("Sun_Directional")
            try:
                lc = sun.get_editor_property("light_component")
                lc.set_editor_property("intensity", 3.0)
            except Exception as e:
                unreal.log_warning("sun intensity: %s" % e)

        # --- B: real-time SkyLight (no static build) ---
        sky = actor_sub.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 800))
        if sky:
            sky.set_actor_label("SkyLight_RT")
            for prop in ("real_time_capture", "b_real_time_capture"):
                try:
                    sky.set_editor_property(prop, True)
                    break
                except Exception:
                    pass
            try:
                sky.set_editor_property("intensity_scale", 1.0)
            except Exception:
                pass

        # --- B: SkyAtmosphere (blue sky for the SkyLight to capture) ---
        atm = actor_sub.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))
        if atm:
            atm.set_actor_label("Sky_Atmosphere")

        # --- B: PlayerStart ---
        ps = actor_sub.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0, 0, 120))
        if ps:
            ps.set_actor_label("PlayerStart_0")

        unreal.EditorLevelLibrary.save_current_level()
        unreal.log("setup_base_scene B done: ground+sun+skylight+atmo+playerstart")

        # --- A: attempt a lighting build (best effort, non-fatal) ---
        try:
            world = editor.get_editor_world()
            unreal.SystemLibrary.execute_console_command(world, "BuildAllLevels")
            unreal.log("setup_base_scene A: lighting build requested")
        except Exception as e:
            unreal.log_warning("lighting build skip: %s" % e)

        unreal.EditorLevelLibrary.save_current_level()
        with open(DONE, "w") as f:
            f.write("ok\n")

    except Exception as e:
        with open(ERR, "w") as f:
            f.write(traceback.format_exc())
        unreal.log_error("setup_base_scene ERROR: %s" % e)


if __name__ == "__main__":
    main()
