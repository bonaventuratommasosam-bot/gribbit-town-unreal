"""
final_lit.py -- Gribbit Town (canon project: Documents/design)
Final attempt: build a lit scene and FORCE the active viewport to LIT mode via
the console command (most reliable in 5.5), not the viewport subsystem.
  - ground (lit grey)
  - MOVABLE DirectionalLight intensity 10, no shadows
  - PlayerStart
  - console: 'viewmode lit' + 'r.DefaultFeature.AutoExposure 0' + brightness
Saves twice.
"""

import unreal
import traceback

MAP = "/Game/Maps/GribbitTown_Main"
DONE = "C:/Users/pc/Documents/design/final_lit_done.marker"
ERR = "C:/Users/pc/Documents/design/final_lit_error.log"
LABELS = ("Ground_Town", "Ground_Fix", "Sun_Directional", "SkyLight_RT",
          "Sky_Atmosphere", "Ground_NoSky", "Sun_NoSky", "Ground_Diag", "Sun_Diag",
          "Ground_Final", "Sun_Final")


def main():
    try:
        editor = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
        actor_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        if not unreal.EditorAssetLibrary.does_asset_exist(MAP):
            raise RuntimeError("Map %s not found" % MAP)
        editor.load_level(MAP)

        for a in actor_sub.get_all_level_actors():
            if a.get_actor_label() in LABELS:
                actor_sub.destroy_actor(a)

        # Ground.
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
            ground.set_actor_label("Ground_Final")

        # Sun.
        sun = actor_sub.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 700))
        if sun:
            sun.set_actor_rotation(unreal.Rotator(-60, 45, 0), False)
            sun.set_actor_label("Sun_Final")
            lc = sun.get_editor_property("light_component")
            lc.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
            lc.set_editor_property("intensity", 10.0)
            lc.set_editor_property("cast_shadows", False)

        # PlayerStart.
        ps = actor_sub.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0, 0, 120))
        if ps:
            ps.set_actor_label("PlayerStart_0")

        world = unreal.EditorLevelLibrary.get_editor_world()

        # Force LIT via console command (targets active viewport).
        unreal.SystemLibrary.execute_console_command(world, "viewmode lit")
        unreal.SystemLibrary.execute_console_command(world, "r.DefaultFeature.AutoExposure 0")
        unreal.SystemLibrary.execute_console_command(world, "r.EyeAdaptationQuality 0")
        unreal.log("final_lit: viewmode lit forced")

        unreal.EditorLevelLibrary.save_current_level()
        # re-assert after save
        unreal.SystemLibrary.execute_console_command(world, "viewmode lit")
        unreal.EditorLevelLibrary.save_current_level()
        with open(DONE, "w") as f:
            f.write("ok\n")

    except Exception as e:
        import traceback as _tb
        with open(ERR, "w") as f:
            f.write(_tb.format_exc())
        unreal.log_error("final_lit ERROR: %s" % e)


if __name__ == "__main__":
    main()
