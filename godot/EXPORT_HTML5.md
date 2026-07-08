# Guida all'Esportazione HTML5 di Gribbits Town

## Requisiti
- Godot 4.3 o superiore
- Browser moderno (Chrome, Firefox, Edge)

## Passi Dettagliati

1. Apri il progetto `godot/` in Godot
2. Vai su **Project → Export**
3. Aggiungi una nuova preset di tipo **Web**
4. Configura la preset:
   - **Export Path**: `export/web/index.html`
   - **Threads**: Abilita (se supportato dal browser)
   - **Compression**: Gzip o Brotli (se il server lo supporta)
5. Clicca **Export Project**
6. La cartella `export/web/` conterrà:
   - `index.html`
   - `index.js`
   - `index.wasm`
   - `index.pck`

7. Carica questi file sul tuo sito web (stessa cartella)

## Consigli per Buone Performance

- Mantieni il numero di nodi basso
- Usa mesh semplici (primitive sono perfette)
- Evita troppe luci dinamiche
- Testa spesso l'esportazione
- Per mondi grandi, dividi in scene più piccole

## Note
Il progetto attuale è già abbastanza ottimizzato perché usa mesh semplici e script leggeri.

*Guida creata per Gribbits Town*