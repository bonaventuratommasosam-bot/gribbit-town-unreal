"""
scene_no_skylight.py -- Gribbit Town (canon project: Documents/design)
B (final): a scene that NEVER needs a real-time SkyLight capture, so it cannot
get stuck 'waiting on shaders' on the Intel UHD 620.
  - ground plane with standard lit grey material
  - strong MOVABLE DirectionalLight (sun) -- always works on Intel
  - ambient color (sky/ground ambient) set directly so the scene is lit
    even with no SkyLight
  - PlayerStart
No SkyLight, no SkyAtmosphere. Viewport forced to LIT.
"""

import unreal
import traceback

MAP = "/Game/Maps/GribbitTown_Main"
DONE = "C:/Users/pc/Documents/design/scene_no_skylight_done.marker"
ERR = "C:/Users/pc/Documents/design/scene_no_skylight_error.log"

LABELS = ("Ground_Town", "Ground_Fix", "Sun_Directional", "SkyLight_RT",
          "Sky_Atmosphere", "Ground_NoSky", "Sun_NoSky")


def main():
    try:
        editor = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
        actor_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        if not unreal.EditorAssetLibrary.does_asset_exist(MAP):
            raise RuntimeError("Map %s not found" % MAP)
        editor.load_level(MAP)

        # Remove any previous base-scene actors (incl. stuck SkyLight).
        for a in actor_sub.get_all_level_actors():
            if a.get_actor_label() in LABELS:
                actor_sub.destroy_actor(a)

        # Ground with standard lit grey material.
        ground = actor_sub.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0))
        if ground:
            plane = unreal.load_object(None, "/Engine/BasicShapes/Plane.Plane")
            mat = unreal.load_object(None, "/Engine/EngineMaterials/DefaultMaterial.DefaultMaterial")
            smc = ground.static_mesh_component
            if plane and smc:
                smc.set_editor_property("static_mesh", plane)
                ground.set_actor_scale3d(unreal.Vector(120, 120, 1))
                if mat:
                    smc.set_editor_property("override_materials", [mat])
            ground.set_actor_label("Ground_NoSky")

        # Strong Movable sun (no SkyLight dependency).
        sun = actor_sub.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 700))
        if sun:
            sun.set_actor_rotation(unreal.Rotator(-60, 45, 0), False)
            sun.set_actor_label("Sun_NoSky")
            try:
                sun.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
            except Exception:
                pass
            try:
                lc = sun.get_editor_property("light_component")
                lc.set_editor_property("intensity", 6.0)
                # visible specular-ish look, no shadows needed for visibility
                lc.set_editor_property("cast_shadows", True)
            except Exception as e:
                unreal.log_warning("sun light prop: %s" % e)

        # PlayerStart.
        ps = actor_sub.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0, 0, 120))
        if ps:
            ps.set_actor_label("PlayerStart_0")

        # Force LIT viewport.
        try:
            vp = unreal.get_editor_subsystem(unreal.EditorViewportSubsystem)
            vp.set_view_mode(unreal.ViewModeIndex.LIT)
        except Exception as e:
            unreal.log_warning("viewport lit: %s" % e)

        unreal.EditorLevelLibrary.save_current_level()
        try:
            vp = unreal.get_editor_subsystem(unreal.EditorViewportSubsystem)
            vp.set_view_mode(unreal.ViewModeIndex.LIT)
        except Exception:
            pass
        unreal.EditorLevelLibrary.save_current_level()
        with open(DONE, "w") as f:
            f.write("ok\n")

    except Exception as e:
        with open(ERR, "w") as f:
            f.write(traceback.format_exc())
        unreal.log_error("scene_no_skylight ERROR: %s" % e)


if __name__ == "__main__":
    main()
