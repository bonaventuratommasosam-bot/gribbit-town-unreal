"""fix_map_gamemode.py — run INSIDE the editor (headless OK).
Sets GribbitTown_Main's World Settings GameMode Override to
AGribbitTownGameMode so PIE/standalone uses our GameMode (spawns frogs + HUD).
"""
import unreal

MAP = "/Game/Maps/GribbitTown_Main"

def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP):
        unreal.log_error(f"{MAP} not found")
        return
    unreal.EditorLevelLibrary.load_level(MAP)
    world = unreal.EditorLevelLibrary.get_editor_world()
    ws = world.get_world_settings()
    gm_class = unreal.load_object(None, "/Script/GribbitTown.GribbitTownGameMode")
    ws.set_editor_property("game_mode_override", gm_class)
    unreal.EditorLevelLibrary.save_current_level()
    unreal.log(f"Set GameMode Override -> {gm_class.get_path_name()}")

if __name__ == "__main__":
    main()
