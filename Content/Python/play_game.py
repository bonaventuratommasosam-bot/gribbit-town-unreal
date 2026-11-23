"""
play_game.py  --  Gribbit Town (canon project: Documents/design)
Loads GribbitTown_Main and starts Play-In-Editor (PIE) so the game is running
as soon as the editor window opens. Uses the modern LevelEditorSubsystem to
load the level; PIE is started via EditorLevelLibrary (works in 5.5).
"""

import unreal

MAP_PATH = "/Game/Maps/GribbitTown_Main"


def main():
    subsys = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        subsys.load_level(MAP_PATH)
        unreal.log("play_game: loaded %s" % MAP_PATH)
    else:
        unreal.log_error("play_game: map %s not found" % MAP_PATH)
        return

    # Start Play-In-Editor (PIE). Deprecated but functional in 5.5.
    try:
        unreal.EditorLevelLibrary.play_in_editor()
        unreal.log("play_game: PIE started")
    except Exception as e:
        unreal.log_error("play_game: failed to start PIE: %s" % e)


if __name__ == "__main__":
    main()
