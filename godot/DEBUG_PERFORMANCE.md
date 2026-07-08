# Debug Performance nel Browser (Godot HTML5)

## Strumenti Principali

### 1. Godot Profiler (durante lo sviluppo)
- Apri il **Profiler** dal menu in alto (o `Ctrl + Shift + P`)
- Monitora:
  - Frame Time
  - Script
  - Physics
  - Rendering
- Utile per individuare colli di bottiglia prima dell'esportazione

### 2. Browser Developer Tools

#### Chrome / Edge / Firefox:
- Premi `F12` o `Ctrl + Shift + I`
- Vai nella scheda **Performance**
- Registra una sessione di gioco
- Analizza:
  - FPS
  - CPU usage
  - Memory
  - Rendering

#### Console del Browser:
Puoi scrivere comandi JavaScript per ottenere statistiche da Godot:

```js
// Mostra FPS approssimativo
console.log('FPS approx:', 1000 / performance.now());
```

### 3. Statistiche Godot in HTML5

Nel browser puoi attivare le statistiche di debug aggiungendo parametri all'URL:

```
index.html?debug=true
```

Oppure usa la console del browser e digita:

```js
GodotEngine.set_debug(true);
```

### 4. Metriche Importanti da Monitorare

- **FPS** (idealmente stabile a 60 o almeno 30)
- **Frame Time** (sotto 16ms per 60 FPS)
- **Memory Usage** (non deve crescere continuamente)
- **Draw Calls** (evita di superare 100-200 su web)
- **Script Time** (script pesanti rallentano tutto)

### 5. Problemi Comuni e Soluzioni

| Problema              | Possibile Causa               | Soluzione                          |
|-----------------------|-------------------------------|------------------------------------|
| FPS basso             | Troppi nodi / luci            | Riduci nodi attivi                 |
| Stuttering            | Caricamento pesante           | Precarica o usa streaming          |
| Memoria che cresce    | Memory leak                   | Controlla script e nodi            |
| Input lag             | Script pesanti nel process    | Ottimizza `_process()`             |

### 6. Consigli Pratici

- Testa sempre l'esportazione HTML5 durante lo sviluppo
- Inizia con scene semplici
- Usa il Profiler di Godot regolarmente
- Monitora la console del browser per errori
- Per mondi grandi, considera di dividere in scene multiple

## Il Nostro Progetto
Attualmente Gribbits Town usa mesh semplici e script leggeri, quindi dovrebbe performare bene su web. Continua a testare l'esportazione man mano che aggiungi contenuti.

*Guida specifica per Gribbits Town*