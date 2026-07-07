Gribbits Town è un life-sim 3D in stile low-poly ambientato in una città di rane (GRiBBiTS). I giocatori esplorano, socializzano e gestiscono i bisogni (fame, energia, divertimento) dei loro frog.

## Stato Attuale (MVP)
- Movimento base (WASD + camera)
- Sistema bisogni base (GribbitNeedsComponent)
- Sistema outfit (GribbitOutfitComponent)
- Generazione procedurale semplice della città

## Prossimi Passi
- [x] UI per le barre dei bisogni — C++ base (UGribbitNeedsWidget) + WBP_NeedsBars script
- [x] Sistema di interazione semplice — UGribbitInteractionComponent + AGribbitInteractable
- [x] Sostituire la generazione procedurale con level design vero — GameMode carica GribbitTown_Main + build_level.py
- [x] 7 personaggi iconici — DataTable DT_Characters + 7 Blueprint generati da setup_characters.py
- [x] Multiplayer (foundation) — AGribbitPlayerState/Controller, replicated needs, listen-server
- [ ] Sistema di outfit più ricco (Pizza, Cowboy, MARFA, sport, ecc.)
- [ ] Audio e musica (jazz, bossa nova)
- [ ] Veicoli (macchine, barche)
- [ ] Eventi dinamici e NPC

## Foundation implementata (C++ — richiede toolchain per compilare)
Vedi `TOOLCHAIN.md` / `BUILD_NOW.md`. Dopo aver installato VS2022 + Windows SDK
e compilato, lancia i 3 script Python in `Content/Python/` per generare i
Blueprint, la UI e la mappa.
