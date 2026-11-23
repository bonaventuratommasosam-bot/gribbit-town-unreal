"""
diag_level.py -- Gribbit Town (canon project: Documents/design)
Reads back the ACTUAL state of the level: every actor, its class, transform,
and (for lights) intensity / mobility / affects_world. Writes a plain report
to diag_report.txt so we stop guessing.
"""

import unreal
import traceback

MAP = "/Game/Maps/GribbitTown_Main"
OUT = "C:/Users/pc/Documents/design/diag_report.txt"
ERR = "C:/Users/pc/Documents/design/diag_error.log"


def main():
    try:
        editor = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
        actor_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        editor.load_level(MAP)

        lines = []
        lines.append("=== LEVEL: %s ===" % MAP)
        lines.append("actor count: %d" % len(actor_sub.get_all_level_actors()))
        lines.append("")

        for a in actor_sub.get_all_level_actors():
            cls = a.get_class().get_name()
            loc = a.get_actor_location()
            lbl = a.get_actor_label()
            line = "[%s] '%s' loc=(%.1f,%.1f,%.1f)" % (cls, lbl, loc.x, loc.y, loc.z)
            if cls in ("DirectionalLight", "PointLight", "SkyLight", "SpotLight"):
                try:
                    lc = a.get_editor_property("light_component")
                    intensity = lc.get_editor_property("intensity")
                    mob = lc.get_editor_property("mobility")
                    affects = lc.get_editor_property("affects_world")
                    line += " | intensity=%s mobility=%s affects_world=%s" % (intensity, mob, affects)
                except Exception as e:
                    line += " | light-read-err: %s" % e
            if cls == "StaticMeshActor":
                try:
                    smc = a.static_mesh_component
                    sm = smc.get_editor_property("static_mesh")
                    smname = sm.get_name() if sm else "NONE"
                    line += " | mesh=%s" % smname
                except Exception as e:
                    line += " | mesh-read-err: %s" % e
            lines.append(line)

        with open(OUT, "w") as f:
            f.write("\n".join(lines) + "\n")

    except Exception as e:
        import traceback as _tb
        with open(ERR, "w") as f:
            f.write(_tb.format_exc())


if __name__ == "__main__":
    main()
