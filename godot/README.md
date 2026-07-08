# Gribbits Town - Godot Web Edition

## Obiettivo
Sandbox con rane antropomorfe (Gribbits) **giocabile direttamente nel browser** dal tuo sito.

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

## Come Provare Localmente

1. Apri la cartella `godot` con Godot 4.3+
2. Apri `MainMenu.tscn` o `MainWorld.tscn`
3. Premi Play

## Ottimizzazione per HTML5 / Web Export

### Passi per Esportare in HTML5

1. Apri il progetto in Godot 4.3+
2. Vai su **Project → Export**
3. Clicca **Add...** e scegli **Web**
4. Nella preset **Web** configura:
   - **Export Path**: `export/web/index.html`
   - **Threads**: Abilita se possibile (migliora performance)
   - **Extensions Support**: Lascia di default
   - **Progressive Web App (PWA)**: Opzionale
5. Clicca **Export Project**
6. Carica la cartella `export/web/` sul tuo hosting web

### Consigli per Ottimizzare le Performance su Web

- Mantieni le scene leggere (evita troppi nodi complessi)
- Usa mesh semplici (attualmente usiamo primitive)
- Limita il numero di luci attive
- Evita shader complessi
- Testa spesso l'esportazione durante lo sviluppo
- Per scene grandi, considera Level of Detail (LOD) o streaming

### Limitazioni HTML5
- Performance inferiori rispetto a desktop
- Alcuni input (es. gamepad) possono variare
- File size deve rimanere ragionevole

*Versione pensata per essere giocata direttamente dal browser*