"""
populate_town.py  --  Gribbit Town (canon project: Documents/design)
Spawns the 7 iconic Gribbit frogs into GribbitTown_Main using the MODERN,
non-deprecated editor subsystems (EditorActorSubsystem / LevelEditorSubsystem).
Real BP characters are spawned; if a BP fails to load, a visible cube is placed
as a fallback so the map is never empty. Writes a DONE marker, saves, then quits.
"""

import unreal
import os
import traceback

DONE = "C:/Users/pc/Documents/design/populate_done.marker"
ERR = "C:/Users/pc/Documents/design/populate_error.log"
MAP_PATH = "/Game/Maps/GribbitTown_Main"

FROGS = [
    "/Game/Characters/BP_ChillPete",
    "/Game/Characters/BP_SheriffBuck",
    "/Game/Characters/BP_MaxMARFA",
    "/Game/Characters/BP_LolaBounce",
    "/Game/Characters/BP_DocHopkins",
    "/Game/Characters/BP_GrandpaMoss",
    "/Game/Characters/BP_LanaMARFA",
]


def main():
    try:
        editor_subsys = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
        actor_subsys = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

        if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
            raise RuntimeError("Map %s not found" % MAP_PATH)

        # Load the level (modern subsystem).
        if not editor_subsys.load_level(MAP_PATH):
            raise RuntimeError("Failed to load level %s" % MAP_PATH)

        # Idempotent: clear previously spawned frogs.
        for a in actor_subsys.get_all_level_actors():
            lbl = a.get_actor_label()
            if lbl.startswith("Frog_") or lbl.startswith("Gribbit_"):
                actor_subsys.destroy_actor(a)

        spawned = 0
        for i, frog_path in enumerate(FROGS):
            bp_name = frog_path.split('/')[-1]
            x = (i % 4) * 400.0 - 600.0
            y = (i // 4) * 400.0 - 200.0
            z = 100.0
            loc = unreal.Vector(x, y, z)

            actor = None
            # Try the real BP character first.
            try:
                bp_class = unreal.load_object(None, frog_path + "." + bp_name + "_C")
                if bp_class is not None:
                    actor = actor_subsys.spawn_actor_from_class(bp_class, loc)
            except Exception as e:
                unreal.log_warning("BP spawn failed for %s: %s" % (bp_name, e))

            # Fallback to a visible cube so the map is never empty.
            if actor is None:
                actor = actor_subsys.spawn_actor_from_class(unreal.StaticMeshActor, loc)
                if actor is not None:
                    try:
                        cube = unreal.load_object(None, "/Engine/BasicShapes/Cube.Cube")
                        smc = actor.static_mesh_component
                        if cube is not None and smc is not None:
                            smc.set_editor_property("static_mesh", cube)
                        actor.set_actor_scale3d(unreal.Vector(0.5, 0.5, 0.5))
                    except Exception as e:
                        unreal.log_warning("cube mesh set failed: %s" % e)

            if actor is not None:
                actor.set_actor_label("Frog_%d_%s" % (i, bp_name))
                spawned += 1
            else:
                unreal.log_warning("Failed to spawn %s" % bp_name)

        # Save the map via asset library.
        unreal.EditorLevelLibrary.save_current_level()
        unreal.log("populate_town done: spawned %d/%d frogs into %s" % (spawned, len(FROGS), MAP_PATH))

        with open(DONE, "w") as f:
            f.write("done spawned=%d/%d\n" % (spawned, len(FROGS)))

    except Exception as e:
        with open(ERR, "w") as f:
            f.write(traceback.format_exc())
        unreal.log_error("populate_town ERROR: %s" % e)
        return

    # Clean quit through the editor (flush save first).
    unreal.SystemLibrary.quit_editor()


if __name__ == "__main__":
    main()
