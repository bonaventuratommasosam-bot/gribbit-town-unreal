# Gribbits Town - Godot Web Edition

## Obiettivo
Sandbox con rane antropomorfe (Gribbits) **giocabile direttamente nel browser**.

## Stato Attuale

### Funzionalità Implementate
- [x] Menu principale
- [x] Movimento prima persona con mouse look
- [x] Sistema di costruzione con preview e grid snap
- [x] 4 colori di blocchi diversi (1-4)
- [x] Piazza e rimuovi blocchi
- [x] Sistema Bisogni completo (Fame, Energia, Divertimento, Social)
- [x] UI visibile dei bisogni
- [x] Interazione (E) che ripristina la fame
- [x] Mondo con alberi

## Controlli
- **WASD** → Muoviti
- **Mouse** → Guarda intorno
- **Spazio** → Salta
- **Click Sinistro** → Piazza blocco
- **Click Destro** → Rimuovi blocco
- **1 / 2 / 3 / 4** → Cambia colore del blocco
- **E** → Interagisci

## Come Provare

1. Apri la cartella `godot` con Godot 4.3+
2. Apri `MainMenu.tscn` o `MainWorld.tscn`
3. Premi Play

## Ottimizzazione Asset 3D per Web

Vedi il file `3D_ASSET_OPTIMIZATION.md` per una guida completa su come ottimizzare mesh, texture e materiali per l'esportazione HTML5.

## Debug Performance nel Browser

Vedi il file `DEBUG_PERFORMANCE.md` per monitorare le performance quando il gioco gira nel browser.

## Come Esportare in HTML5

1. Project → Export
2. Aggiungi preset **Web**
3. Configura Threads e Compression
4. Export Project
5. Carica i file sul tuo sito

*Versione pensata per essere giocata direttamente dal browser*