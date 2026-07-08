# Ottimizzazione Asset 3D per Web (Godot HTML5)

## Introduzione
Quando esporti un gioco Godot per il web, gli asset 3D sono uno dei fattori che influenzano maggiormente le performance e le dimensioni del download.

## 1. Mesh Optimization

### Riduci il numero di poligoni
- Usa mesh a bassa risoluzione quando possibile
- Per personaggi e oggetti dettagliati, crea versioni Low-Poly
- Evita mesh con migliaia di vertici se non strettamente necessario

### Usa Level of Detail (LOD)
Godot supporta LOD nativo:
- Crea diverse versioni della stessa mesh (High / Medium / Low)
- Godot le scambierà automaticamente in base alla distanza

### Merge Mesh
- Combina più mesh statiche in una sola quando possibile
- Riduce il numero di draw call

## 2. Texture Optimization

### Formati Consigliati per il Web
- **Basis Universal** (migliore compressione per web)
- **ETC2** o **ASTC** per dispositivi mobili
- Evita texture molto grandi (massimo 1024x1024 o 2048x2048 per web)

### Tecniche
- Usa **texture atlases** (una texture grande invece di tante piccole)
- Comprimi le texture durante l'import in Godot
- Riduci la risoluzione delle texture per oggetti lontani

## 3. Materiali e Shader

### Materiali Semplici
- Preferisci materiali base (`StandardMaterial3D`)
- Evita shader custom complessi
- Usa il minor numero possibile di materiali diversi

## 4. Import Settings in Godot

Quando importi un modello 3D:
- **Mesh**:
  - Abilita "Generate LOD" se disponibile
  - Usa "Compress Mesh" quando possibile
- **Textures**:
  - Imposta il formato di compressione su Basis Universal
  - Riduci la risoluzione se non necessaria

## 5. Best Practices Generali per Web

- Inizia con asset semplici (il nostro progetto usa primitive, che sono perfette)
- Aggiungi complessità gradualmente
- Testa spesso l'esportazione HTML5
- Per i 7 Gribbits, crea versioni Low-Poly
- Evita texture 4K o molto dettagliate

## Stato Attuale del Nostro Progetto
Attualmente Gribbits Town usa:
- Mesh primitive (Cube, Sphere, Cylinder) → **Ottimo per web**
- Pochi materiali
- Nessuna texture complessa

Questo significa che il progetto è già ben posizionato per una buona performance su browser.

Quando aggiungeremo i 7 Gribbits come modelli 3D, dovremo applicare queste tecniche di ottimizzazione.

*Guida specifica per Gribbits Town*