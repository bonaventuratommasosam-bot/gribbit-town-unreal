# Come Giocare a Gribbits Town (Versione Attuale)

## Requisiti
- Unreal Engine 5.5
- Visual Studio 2022 + Windows SDK (per compilare il C++)
- Il progetto deve essere compilato almeno una volta

## Passi per Giocare

### 1. Compila il progetto
Apri `GribbitTown.uproject` con Unreal Engine 5.5.
Vai su **Tools → Refresh Visual Studio Project** se necessario.
Compila il modulo `GribbitTown` (Development Editor).

### 2. Genera la mappa (consigliato)
Esegui lo script:
```
Content/Python/build_level.py
```
O usa il file `.bat` se presente.

### 3. Apri la mappa
Apri `Content/Maps/GribbitTown_Main.umap`

### 4. Avvia in Modalità Gioco
- Premi **Play** (o Alt + P)
- Muoviti con **WASD**
- La telecamera ti segue

### 5. Gioca in Modalità Online (Listen Server)
1. Avvia il gioco in modalità **Listen Server** (dal menu o da console).
2. Un altro giocatore può unirsi usando l'IP del server.

## Controlli Attuali
- **WASD** → Muoviti
- **Mouse** → Guarda intorno
- Interazione e costruzione sono in fase di sviluppo

## Prossimi Obiettivi (per renderlo più giocabile)
- Sistema di costruzione funzionante
- I 7 Gribbits selezionabili
- Bisogni visibili con UI
- Oggetti con cui interagire

*Guida creata per arrivare a una versione giocabile il prima possibile*