"""
light_diagnostic.py -- Gribbit Town (canon project: Documents/design)
Sets a MOVABLE DirectionalLight by configuring the LIGHT COMPONENT (not the
actor) so it lights in real-time with no build. Then reads back the actual
values and logs them, so we stop guessing. Also a lit ground + PlayerStart.
"""

import unreal
import traceback

MAP = "/Game/Maps/GribbitTown_Main"
DONE = "C:/Users/pc/Documents/design/light_diag_done.marker"
ERR = "C:/Users/pc/Documents/design/light_diag_error.log"
LABELS = ("Ground_Town", "Ground_Fix", "Sun_Directional", "SkyLight_RT",
          "Sky_Atmosphere", "Ground_NoSky", "Sun_NoSky", "Ground_Diag", "Sun_Diag")


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

        # Ground (lit grey).
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
            ground.set_actor_label("Ground_Diag")

        # DirectionalLight: configure the COMPONENT.
        sun = actor_sub.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 700))
        if sun:
            sun.set_actor_rotation(unreal.Rotator(-60, 45, 0), False)
            sun.set_actor_label("Sun_Diag")
            lc = sun.get_editor_property("light_component")
            lc.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
            lc.set_editor_property("intensity", 10.0)
            lc.set_editor_property("cast_shadows", False)  # simplest: no shadow pass
            # read back to verify
            ri = lc.get_editor_property("intensity")
            rm = lc.get_editor_property("mobility")
            rv = lc.get_editor_property("b_visible") if hasattr(lc, "b_visible") else "n/a"
            unreal.log("SUN_DIAG intensity=%s mobility=%s visible=%s" % (ri, rm, rv))

        # PlayerStart.
        ps = actor_sub.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0, 0, 120))
        if ps:
            ps.set_actor_label("PlayerStart_0")

        # Force LIT.
        try:
            vp = unreal.get_editor_subsystem(unreal.EditorViewportSubsystem)
            vp.set_view_mode(unreal.ViewModeIndex.LIT)
        except Exception as e:
            unreal.log_warning("vp lit: %s" % e)

        unreal.EditorLevelLibrary.save_current_level()
        # trigger a real-time preview light refresh
        try:
            world = editor.get_editor_world()
            unreal.SystemLibrary.execute_console_command(world, "RebuildLighting")
            unreal.log("SUN_DIAG: requested RebuildLighting")
        except Exception as e:
            unreal.log_warning("rebuild: %s" % e)

        unreal.EditorLevelLibrary.save_current_level()
        with open(DONE, "w") as f:
            f.write("ok\n")

    except Exception as e:
        import traceback as _tb
        with open(ERR, "w") as f:
            f.write(_tb.format_exc())
        unreal.log_error("light_diagnostic ERROR: %s" % e)


if __name__ == "__main__":
    main()
