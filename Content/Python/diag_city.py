"""Quanti attori ci sono davvero nel livello, e dove stanno."""

import unreal

LEVEL = "/Game/Maps/GribbitCity"

levels = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
actors_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

loaded = levels.load_level(LEVEL)
unreal.log_warning("load_level(%s) -> %s" % (LEVEL, loaded))

all_actors = actors_sub.get_all_level_actors()
unreal.log_warning("attori totali: %d" % len(all_actors))

kinds = {}
for a in all_actors:
    k = a.get_class().get_name()
    kinds[k] = kinds.get(k, 0) + 1
for k in sorted(kinds, key=lambda x: -kinds[x]):
    unreal.log_warning("   %-28s %d" % (k, kinds[k]))

mins = [1e9, 1e9, 1e9]
maxs = [-1e9, -1e9, -1e9]
n_mesh = 0
for a in all_actors:
    if not isinstance(a, unreal.StaticMeshActor):
        continue
    comp = a.static_mesh_component
    if not comp or not comp.static_mesh:
        continue
    n_mesh += 1
    loc = a.get_actor_location()
    for i, v in enumerate((loc.x, loc.y, loc.z)):
        mins[i] = min(mins[i], v)
        maxs[i] = max(maxs[i], v)

unreal.log_warning("static mesh actor con mesh assegnata: %d" % n_mesh)
if n_mesh:
    unreal.log_warning("estensione citta: X %.0f..%.0f  Y %.0f..%.0f  Z %.0f..%.0f"
               % (mins[0], maxs[0], mins[1], maxs[1], mins[2], maxs[2]))
    unreal.log_warning("centro: %.0f, %.0f" % ((mins[0] + maxs[0]) / 2, (mins[1] + maxs[1]) / 2))
