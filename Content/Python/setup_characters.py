"""
setup_characters.py  —  Gribbits Town asset generator (run INSIDE the Unreal Editor)

Run from the editor:  Outputs > Python > Execute File  ->  select this file.
Requires the C++ module (GribbitTown) to be compiled (Visual Studio 2022 + Windows SDK).

What it does:
  Feature 1: creates 7 Blueprint characters (BP_ChillPete, BP_SheriffBuck,
             BP_MaxMARFA, BP_LolaBounce, BP_DocHopkins, BP_GrandpaMoss,
             BP_LanaMARFA) as children of AGribbitCharacter.
  Feature 3 support: creates the DT_Characters DataTable with one FGribbitCharacterRow
             per frog (display name, bio, outfit, tint, starting needs).

After running, open the GameMode and set DefaultFrogClass = BP_ChillPete.
"""

import unreal
import csv
import io

TARGET_BP_DIR = "/Game/Characters"
DATA_DIR = "/Game/Data"
BP_PARENT = "/Script/GribbitTown.GribbitCharacter"

# The 7 iconic Gribbits (id -> display, bio, outfit, tint RGB, start needs)
CHARACTERS = [
    {
        "id": "ChillPete", "display": "Chill Pete",
        "bio": "The laid-back pizza lover. Always near the pizzeria.",
        "outfit": "Pizza", "tint": (0.4, 0.8, 0.5),
        "needs": (85, 80, 75, 60, 78, 88),
    },
    {
        "id": "SheriffBuck", "display": "Sheriff Buck",
        "bio": "Keeps the peace in Gribbit Town. Wears a star.",
        "outfit": "Cowboy", "tint": (0.7, 0.55, 0.3),
        "needs": (80, 85, 60, 55, 80, 82),
    },
    {
        "id": "MaxMARFA", "display": "Max MARFA",
        "bio": "The cool DJ frog of MARFA district. Loves to party.",
        "outfit": "MARFA", "tint": (0.5, 0.4, 0.9),
        "needs": (78, 70, 90, 75, 65, 80),
    },
    {
        "id": "LolaBounce", "display": "Lola Bounce",
        "bio": "Energetic athlete frog. Never stops bouncing.",
        "outfit": "Sport", "tint": (0.9, 0.4, 0.6),
        "needs": (75, 75, 80, 70, 70, 78),
    },
    {
        "id": "DocHopkins", "display": "Doc Hopkins",
        "bio": "The town doctor. Calm and clever.",
        "outfit": "Doctor", "tint": (0.6, 0.7, 0.8),
        "needs": (82, 82, 65, 60, 85, 84),
    },
    {
        "id": "GrandpaMoss", "display": "Grandpa Moss",
        "bio": "The oldest frog in town. Full of stories.",
        "outfit": "Elder", "tint": (0.55, 0.6, 0.45),
        "needs": (70, 65, 55, 50, 72, 70),
    },
    {
        "id": "LanaMARFA", "display": "Lana MARFA",
        "bio": "The artist frog of MARFA. Paints the town.",
        "outfit": "Artist", "tint": (0.9, 0.7, 0.3),
        "needs": (79, 76, 85, 72, 74, 81),
    },
]

DATA_ROW_CLS = "/Script/GribbitTown.GribbitCharacterRow"  # FGribbitCharacterRow


def ensure_dir(path):
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def create_character_bp(char):
    bp_name = "BP_" + char["id"]
    bp_path = f"{TARGET_BP_DIR}/{bp_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(bp_path):
        unreal.log_warning(f"Skip existing {bp_path}")
        return bp_path

    factory = unreal.BlueprintFactory()
    factory.set_editor_property("parent_class", unreal.load_class(None, BP_PARENT))
    asset = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        bp_name, TARGET_BP_DIR, unreal.Blueprint, factory)
    if not asset:
        unreal.log_error(f"Failed to create {bp_path}")
        return None

    # Set the preset ID so ApplyPreset() finds this row in DT_Characters.
    bp = unreal.load_object(None, bp_path)
    cdo = unreal.get_default_object(bp.generated_class())
    cdo.set_editor_property("CharacterPresetID", char["id"])
    unreal.EditorAssetLibrary.save_asset(bp_path)
    unreal.log(f"Created {bp_path}")
    return bp_path


def create_character_data_table():
    dt_path = f"{DATA_DIR}/DT_Characters"
    ensure_dir(DATA_DIR)
    if unreal.EditorAssetLibrary.does_asset_exist(dt_path):
        unreal.log_warning(f"DT_Characters already exists; updating rows.")
        dt = unreal.load_object(None, dt_path)
    else:
        factory = unreal.DataTableFactory()
        factory.set_editor_property("struct", unreal.load_object(None, DATA_ROW_CLS))
        dt = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            "DT_Characters", DATA_DIR, unreal.DataTable, factory)
    if not dt:
        unreal.log_error("Failed to create DT_Characters")
        return

    output = io.StringIO()
    writer = csv.writer(output, lineterminator="\n")
    writer.writerow([
        "Name",
        "DisplayName",
        "Bio",
        "DefaultOutfit",
        "BodyTint",
        "StartHunger",
        "StartEnergy",
        "StartFun",
        "StartSocial",
        "StartHygiene",
        "StartBladder",
    ])
    for char in CHARACTERS:
        r, g, b = char["tint"]
        writer.writerow([
            char["id"],
            char["display"],
            char["bio"],
            char["outfit"],
            f"(R={r},G={g},B={b},A=1.0)",
            char["needs"][0],
            char["needs"][1],
            char["needs"][2],
            char["needs"][3],
            char["needs"][4],
            char["needs"][5],
        ])

    unreal.DataTableFunctionLibrary.fill_data_table_from_csv_string(dt, output.getvalue())

    unreal.EditorAssetLibrary.save_asset(dt_path)
    unreal.log(f"Created/updated {dt_path} with {len(CHARACTERS)} rows")


def main():
    ensure_dir(TARGET_BP_DIR)
    for char in CHARACTERS:
        create_character_bp(char)
    create_character_data_table()
    unreal.log("setup_characters.py complete. Set GameMode.DefaultFrogClass = BP_ChillPete.")


if __name__ == "__main__":
    main()
