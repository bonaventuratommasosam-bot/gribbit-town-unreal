"""
Import the CC0 Kenney kits (Art/Kenney/**.fbx) into /Game/Kenney/<Category>/.

Run once, from the editor: Tools > Execute Python Script.
Re-running is safe: assets that already exist are skipped unless FORCE is set.
"""

import os
import unreal

FORCE = False

PROJECT_DIR = unreal.Paths.project_dir()
ART_ROOT = os.path.normpath(os.path.join(PROJECT_DIR, "Art", "Kenney"))
CATEGORIES = ["Roads", "Commercial", "Industrial", "Suburban", "Vehicles"]

asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
registry = unreal.AssetRegistryHelpers.get_asset_registry()


def _options():
    opts = unreal.FbxImportUI()
    opts.set_editor_property("import_mesh", True)
    opts.set_editor_property("import_textures", True)
    opts.set_editor_property("import_materials", True)
    opts.set_editor_property("import_as_skeletal", False)
    opts.set_editor_property("mesh_type_to_import", unreal.FBXImportType.FBXIT_STATIC_MESH)
    sm = opts.static_mesh_import_data
    sm.set_editor_property("combine_meshes", True)
    sm.set_editor_property("generate_lightmap_u_vs", True)
    sm.set_editor_property("auto_generate_collision", True)
    sm.set_editor_property("convert_scene", True)
    return opts


def _tasks_for(category):
    folder = os.path.join(ART_ROOT, category)
    if not os.path.isdir(folder):
        unreal.log_warning("cartella mancante: %s" % folder)
        return []
    dest = "/Game/Kenney/%s" % category
    tasks = []
    for name in sorted(os.listdir(folder)):
        if not name.lower().endswith(".fbx"):
            continue
        asset_path = "%s/%s" % (dest, os.path.splitext(name)[0])
        if not FORCE and unreal.EditorAssetLibrary.does_asset_exist(asset_path):
            continue
        t = unreal.AssetImportTask()
        t.set_editor_property("filename", os.path.join(folder, name))
        t.set_editor_property("destination_path", dest)
        t.set_editor_property("automated", True)
        t.set_editor_property("save", True)
        t.set_editor_property("replace_existing", True)
        t.set_editor_property("options", _options())
        tasks.append(t)
    return tasks


def run():
    total = 0
    for category in CATEGORIES:
        tasks = _tasks_for(category)
        if not tasks:
            unreal.log("%-12s niente da importare" % category)
            continue
        asset_tools.import_asset_tasks(tasks)
        done = sum(len(t.get_editor_property("imported_object_paths") or []) for t in tasks)
        total += done
        unreal.log("%-12s importati %d / %d" % (category, done, len(tasks)))
    unreal.EditorAssetLibrary.save_directory("/Game/Kenney", only_if_is_dirty=True, recursive=True)
    unreal.log("FATTO: %d modelli nel progetto" % total)
    return total


if __name__ == "__main__":
    run()
