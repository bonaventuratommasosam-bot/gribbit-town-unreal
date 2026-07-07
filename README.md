# Gribbits Town - Unreal Edition

**Gribbits Town** è un videogioco di simulazione di vita stile The Sims con rane antropomorfe stylish, ambientato in una cittadina palustre moderna e vivace.

Il progetto è in fase iniziale su Unreal Engine 5.5 (C++).

## Visione del Gioco

- **Genere**: Life Simulation + Social Sandbox (The Sims + Animal Crossing)
- **Protagonisti**: Rane antropomorfe (Gribbits) con personalità forti
- **Ambientazione**: Gribbits Town — una cittadina su ninfee con quartieri residenziali, centro commerciale, porto e zona mistica
- **Obiettivo a lungo termine**: Esperienza multiplayer online persistente (case visitabili, eventi di paese, economia player-driven)

## Personaggi Principali (Iconici)

Abbiamo definito 7 personaggi principali basati sulle immagini generate:

| Nome              | Ruolo                              | Tipo                  |
|-------------------|------------------------------------|-----------------------|
| Chill Pete        | Proprietario Pizzeria              | Preset + NPC         |
| Sheriff Buck      | Sceriffo                           | Preset + NPC         |
| Max MARFA         | Influencer / Ambasciatore brand    | Preset + NPC         |
| Captain Finn      | Capitano del porto                 | Preset + NPC         |
| Elder Moss        | Saggio del paese                   | NPC Guida            |
| Lord Tiberius     | Tycoon losco                       | NPC con rami morali  |
| Lana MARFA        | Modella / Ambasciatrice ufficiale  | NPC Ricorrente       |

Le immagini ufficiali dei personaggi sono in `References/Characters/`

## Struttura Attuale del Progetto

- `GribbitTown.uproject` — UE 5.5
- `Source/GribbitTown/` — C++ base (GameMode che genera proceduralmente la città + Character base)
- `References/` — Concept art e riferimenti visivi
- `Docs/` — Documentazione di design

## Prossimi Passi (Roadmap)

1. **Personaggi**
   - Importare/Modellare i 7 personaggi
   - Creare Blueprint o C++ per Character con outfit system
   - Sistema Create-A-Gribbit

2. **Mondo**
   - Sostituire la generazione procedurale con level design manuale + streaming
   - Aggiungere interni delle case e negozi

3. **Gameplay Core**
   - Sistema Bisogni (Fame, Energia, Divertimento, Sociale... versione rana)
   - Skill e Carriere
   - Interazioni sociali

4. **Multiplayer (fase successiva)**
   - Valutare Unreal Multiplayer o plugin come Advanced Sessions

## Come Contribuire

Apri una issue o una pull request. Il progetto è aperto a idee!

---

*Creato con Grok by xAI - Documentazione aggiornata Luglio 2026*