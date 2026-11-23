"""
Scatta le foto della citta dall'editor gia' avviato e gia' in disegno.
Va lanciato COSI' (non con -ExecutePythonScript, che chiude l'editor prima
che abbia disegnato qualcosa):

  UnrealEditor.exe <project> -ExecCmds="py <percorso di questo file>"

I PNG finiscono in <progetto>/Saved/Screenshots/WindowsEditor/.
"""

import unreal

LEVEL = "/Game/Maps/GribbitCity"

# strade sui multipli di 4000 uu, citta da (0,0) a circa (24000, 20000)
SHOTS = [
    ("01_aerea",    (-4000, -4000, 17000), (-50,  45, 0)),
    ("02_skyline",  (12000, -7000,  2600), ( -7,  90, 0)),
    ("03_incrocio", (10400, 10400,  4200), (-40,  45, 0)),
    ("04_strada",   ( 5200,  4000,   260), ( -3,   0, 0)),
    ("05_auto",     ( 9400,  8000,   200), ( -4,   8, 0)),
    ("06_isolato",  (14000, 12000,  1500), (-22, 200, 0)),
]

ue = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)
lv = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)

S = {"i": 0, "t": 0, "h": None}

WARMUP = 90          # fotogrammi di attesa iniziale: streaming e shader
SETTLE = 45          # fotogrammi fra spostamento camera e scatto
AFTER = 40           # fotogrammi dopo lo scatto, per lasciarlo scrivere


def console(cmd):
    unreal.SystemLibrary.execute_console_command(ue.get_editor_world(), cmd)


def tick(_delta):
    S["t"] += 1

    if S["t"] < WARMUP:
        return

    step = S["t"] - WARMUP
    per = SETTLE + AFTER
    i = step // per
    sub = step % per

    if i >= len(SHOTS):
        unreal.log_warning("SCATTI COMPLETATI")
        if S["h"]:
            unreal.unregister_slate_post_tick_callback(S["h"])
            S["h"] = None
        console("QUIT_EDITOR")
        return

    name, loc, rot = SHOTS[i]
    if sub == 0:
        ue.set_level_viewport_camera_info(
            unreal.Vector(float(loc[0]), float(loc[1]), float(loc[2])),
            unreal.Rotator(float(rot[2]), float(rot[0]), float(rot[1])))
        unreal.log_warning("inquadratura %s" % name)
    elif sub == SETTLE:
        console("HighResShot 1920x1080 filename=%s" % name)
        unreal.log_warning("scatto %s" % name)


def start():
    lv.load_level(LEVEL)
    try:
        lv.editor_set_game_view(True)
    except Exception as e:
        unreal.log_warning("game view non attivabile: %s" % e)
    console("r.ScreenPercentage 100")
    console("r.Streaming.FullyLoadUsedTextures 1")
    S["h"] = unreal.register_slate_post_tick_callback(tick)
    unreal.log_warning("sequenza avviata: %d inquadrature" % len(SHOTS))


start()
