"""
fix_lighting_view.py -- Gribbit Town (canon project: Documents/design)
Forces the editor viewport to LIT mode, ensures a strong Movable sun +
real-time SkyLight, and gives the ground a standard lit grey material so the
scene is actually visible (not black with wireframe capsules).
"""

import unreal
import traceback

MAP = "/Game/Maps/GribbitTown_Main"
DONE = "C:/Users/pc/Documents/design/fix_lighting_done.marker"
ERR = "C:/Users/pc/Documents/design/fix_lighting_error.log"


def set_view_lit():
    try:
        vp = unreal.get_editor_subsystem(unreal.EditorViewportSubsystem)
        vp.set_view_mode(unreal.ViewModeIndex.LIT)
        unreal.log("fix_lighting: viewport -> LIT")
        return True
    except Exception as e:
        unreal.log_warning("set_view_lit failed: %s" % e)
        return False


def main():
    try:
        editor = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
        actor_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        if not unreal.EditorAssetLibrary.does_asset_exist(MAP):
            raise RuntimeError("Map %s not found" % MAP)
        editor.load_level(MAP)

        set_view_lit()

        # Remove any previous base-scene lights/ground to avoid duplicates.
        for a in actor_sub.get_all_level_actors():
            lbl = a.get_actor_label()
            if lbl in ("Ground_Town", "Sun_Directional", "SkyLight_RT", "Sky_Atmosphere", "Ground_Fix"):
                actor_sub.destroy_actor(a)

        # Ground with standard lit grey material.
        ground = actor_sub.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0))
        if ground:
            plane = unreal.load_object(None, "/Engine/BasicShapes/Plane.Plane")
            mat = unreal.load_object(None, "/Engine/EngineMaterials/DefaultMaterial.DefaultMaterial")
            smc = ground.static_mesh_component
            if plane and smc:
                smc.set_editor_property("static_mesh", plane)
                ground.set_actor_scale3d(unreal.Vector(100, 100, 1))
                if mat:
                    smc.set_editor_property("override_materials", [mat])
            ground.set_actor_label("Ground_Fix")

        # Strong Movable sun.
        sun = actor_sub.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 600))
        if sun:
            sun.set_actor_rotation(unreal.Rotator(-55, 45, 0), False)
            sun.set_actor_label("Sun_Directional")
            try:
                sun.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
            except Exception:
                pass
            try:
                lc = sun.get_editor_property("light_component")
                lc.set_editor_property("intensity", 5.0)
            except Exception as e:
                unreal.log_warning("sun intensity: %s" % e)

        # Real-time SkyLight.
        sky = actor_sub.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 800))
        if sky:
            sky.set_actor_label("SkyLight_RT")
            try:
                sky.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
            except Exception:
                pass
            for prop in ("real_time_capture", "b_real_time_capture", "real_time_skylight_capture"):
                try:
                    sky.set_editor_property(prop, True)
                    break
                except Exception:
                    pass

        # Add sky atmosphere for the skylight to capture a blue sky.
        atm = actor_sub.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))
        if atm:
            atm.set_actor_label("Sky_Atmosphere")

        unreal.EditorLevelLibrary.save_current_level()
        set_view_lit()  # re-assert after load
        unreal.EditorLevelLibrary.save_current_level()
        with open(DONE, "w") as f:
            f.write("ok\n")

    except Exception as e:
        with open(ERR, "w") as f:
            f.write(traceback.format_exc())
        unreal.log_error("fix_lighting ERROR: %s" % e)


if __name__ == "__main__":
    main()
