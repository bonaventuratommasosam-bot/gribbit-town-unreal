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
- [x] Veicoli — auto guidabili (`AGribbitVehicle` + `BP_Car_*`), **da provare in gioco**
- [ ] Sistema di outfit più ricco (Pizza, Cowboy, MARFA, sport, ecc.)
- [ ] Audio e musica (jazz, bossa nova)
- [ ] Barche
- [ ] Eventi dinamici e NPC (adesso i frog nascono e stanno fermi)
- [ ] Modello e animazioni delle rane — il vero collo di bottiglia
- [ ] Menu, salvataggio, suono

## Auto (23 novembre 2026)

`AGribbitVehicle` è un'auto arcade, non un veicolo Chaos: i modelli Kenney
sono mesh statiche senza ossa delle ruote, e una sospensione vera non darebbe
niente a un gioco di guida da sopra la città. Velocità lungo il vettore
avanti, sterzo proporzionale alla velocità, un raycast verso il basso per
restare incollata alla strada.

Comandi: **F** sali sull'auto libera più vicina (entro 4 m), **WASD** guidi,
**E** scendi.

Le auto guidabili le piazza il GameMode all'avvio della partita
(`SpawnDrivableCars`), sostituendo le auto di arredo più vicine al punto di
partenza. Non è una scelta di stile: l'editor headless su questa macchina
crepa sullo spawn di un pawn con telecamera (`INT_DIVIDE_BY_ZERO` con
`-nullrhi`, crash nel D3D11RHI con un renderer vero), e la `.umap` generata
non è versionata — a runtime funziona anche su un clone pulito.

## Stato della macchina (cose che frenano, non bug)

- 8 GB di RAM: l'Unreal Build Accelerator si piantava su "memory pressure".
  Compilare con `-NoUBA -MaxParallelActions=1` (vedi `BUILD_NOW.md`).
- Il motore sta su `D:\EpicGames\UE_5.5`, con una giunzione dal vecchio
  percorso su C: perché l'Epic Launcher e i progetti lo trovino.
- Il progetto sta su `D:` (disco USB meccanico): è lento per la cache degli
  shader, e sull'SSD andrebbe meglio.
- Intel UHD 620: niente rendering fuori schermo, quindi ogni verifica visiva
  passa dalla finestra dell'editor.

## Foundation implementata (C++ — richiede toolchain per compilare)
Vedi `TOOLCHAIN.md` / `BUILD_NOW.md`. Dopo aver installato VS2022 + Windows SDK
e compilato, lancia i 3 script Python in `Content/Python/` per generare i
Blueprint, la UI e la mappa.
