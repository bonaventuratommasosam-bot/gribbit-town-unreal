# Ottimizzazione Godot per WebGL / HTML5

## Introduzione
Godot 4 esporta per il web usando **WebAssembly** (non più WebGL classico). Tuttavia, le performance rimangono inferiori rispetto a desktop.

## Impostazioni di Esportazione Consigliate

### Nella Preset Web:
- **Threads**: Abilita (se il browser lo supporta)
- **Compression**: Gzip o Brotli
- **Progressive Web App (PWA)**: Opzionale
- **Extensions Support**: Lascia di default

## Ottimizzazioni Pratiche

### 1. Grafica
- Usa il renderer **Forward+** (già impostato)
- Evita troppe luci dinamiche
- Usa mesh semplici (primitive sono perfette)
- Evita shader custom pesanti

### 2. Scena
- Mantieni basso il numero di nodi
- Evita nodi inutili
- Per mondi grandi, considera il caricamento di scene separate

### 3. Script
- Evita calcoli pesanti nel `_process()`
- Usa `delta` in modo intelligente
- Evita stringhe complesse ogni frame

### 4. Asset
- Comprimi le texture
- Evita texture molto grandi
- Usa formati efficienti

### 5. Performance
- Usa il **Profiler** di Godot durante lo sviluppo
- Testa spesso l'esportazione HTML5
- Per scene complesse, considera Level of Detail (LOD)

## Il Nostro Progetto Attuale
Il progetto Gribbits Town è già abbastanza ottimizzato perché:
- Usa mesh primitive (molto leggere)
- Ha pochi nodi attivi
- Gli script sono semplici
- Non usa shader complessi

## Consigli Finali
- Inizia con una scena semplice
- Aggiungi complessità gradualmente
- Testa spesso su browser diversi

*Guida specifica per Gribbits Town*