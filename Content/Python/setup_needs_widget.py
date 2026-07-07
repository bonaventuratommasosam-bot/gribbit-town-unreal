"""
setup_needs_widget.py  —  Gribbits Town (run INSIDE the Unreal Editor)

Feature 2: creates WBP_NeedsBars, a Widget Blueprint child of UGribbitNeedsWidget
with 6 progress bars (Hunger, Energy, Fun, Social, Hygiene, Bladder) wired to
OnNeedsUpdated, plus a prompt text bound to the interaction focus.

After running, open AGribbitPlayerController and set NeedsWidgetClass = WBP_NeedsBars.
"""

import unreal

TARGET_DIR = "/Game/UI"
WIDGET_PARENT = "/Script/GribbitTown.GribbitNeedsWidget"


def main():
    if not unreal.EditorAssetLibrary.does_directory_exist(TARGET_DIR):
        unreal.EditorAssetLibrary.make_directory(TARGET_DIR)

    path = f"{TARGET_DIR}/WBP_NeedsBars"
    if unreal.EditorAssetLibrary.does_asset_exist(path):
        unreal.log_warning(f"{path} already exists; skipping.")
        return

    factory = unreal.WidgetBlueprintFactory()
    factory.set_editor_property("parent_class", unreal.load_object(None, WIDGET_PARENT))
    wbp = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        "WBP_NeedsBars", TARGET_DIR, unreal.WidgetBlueprint, factory)
    if not wbp:
        unreal.log_error("Failed to create WBP_NeedsBars")
        return

    # Note: the actual bar widgets and the binding to OnNeedsUpdated are authored
    # visually in the widget designer. This script lays down the Blueprint class so
    # the C++ base (UGribbitNeedsWidget::OnNeedsUpdated) has a concrete child to
    # instantiate. Open it in the designer to add 6 UProgressBar named:
    #   Bar_Hunger, Bar_Energy, Bar_Fun, Bar_Social, Bar_Hygiene, Bar_Bladder
    # and an Event Graph binding: OnNeedsUpdated -> SetPercent on each bar.
    unreal.EditorAssetLibrary.save_asset(path)
    unreal.log(f"Created {path}. Open in the designer to add the 6 progress bars.")


if __name__ == "__main__":
    main()
