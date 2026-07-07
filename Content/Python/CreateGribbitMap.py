# CreateGribbitMap.py
# Runs INSIDE the Unreal Editor (Python scripting is built into UE 5.5).
# Usage (headless):
#   UnrealEditor.exe GribbitTown.uproject -ExecutePythonScript="Content/Python/CreateGribbitMap.py" -unattended -nopause
#
# Creates an empty map, assigns the C++ GameMode, and saves it to
# Content/Maps/GribbitTown.umap. The GameMode builds the whole town at StartPlay,
# so no static geometry needs to live in the map itself.

import unreal

def create_map():
    editor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    # Fresh empty level.
    unreal.EditorLevelLibrary.new_level()

    world = unreal.EditorLevelLibrary.get_editor_world()
    if world is None:
        unreal.log_error("Could not get editor world.")
        return

    # Set the GameMode to our C++ class so StartPlay builds the town.
    world.set_actor_class_override_enabled(False)
    game_mode_path = "/Script/GribbitTown.GribbitTownGameMode"
    game_mode_cdo = unreal.load_object(None, game_mode_path)
    if game_mode_cdo is None:
        # If the module is not loaded yet, fall back to path string assignment.
        unreal.log_warning("GameMode C++ object not found by path; assigning via world settings.")
        world_settings = unreal.EditorLevelLibrary.get_level_actor_for_class(unreal.WorldSettings)
        if world_settings is None:
            spawned = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.WorldSettings, unreal.Vector(0, 0, 0))
            world_settings = spawned
    else:
        world_settings = world.get_world_settings()
        world_settings.set_editor_property("DefaultGameMode", game_mode_cdo)

    # Save the map.
    map_path = "/Game/Maps/GribbitTown"
    options = unreal.SaveAssetOptions()
    result = unreal.EditorAssetLibrary.save_asset(map_path, options)
    if not result:
        # Ensure the Maps directory exists then save.
        unreal.EditorAssetLibrary.make_directory("/Game/Maps")
        result = unreal.EditorAssetLibrary.save_asset(map_path, options)

    unreal.log("GribbitTown map saved to: {}".format(map_path))

create_map()
