# Compressione Texture per WebGL / HTML5 (Godot)

## Introduzione
La compressione delle texture è uno degli aspetti più importanti per ottenere buone performance e tempi di caricamento rapidi quando esporti un gioco Godot per il web.

## Formato Consigliato: Basis Universal

Godot 4 raccomanda **Basis Universal** per l'esportazione web.

### Vantaggi di Basis Universal:
- Ottima compressione (file molto più piccoli)
- Supporto nativo nei browser moderni
- Buona qualità visiva
- Decodifica veloce

### Come Impostarlo in Godot

1. Seleziona una texture nell'editor
2. Nella scheda **Import**:
   - **Compress Mode** → `Basis Universal`
   - **Normal Map** → Abilita solo se è una normal map
   - **HDR Compression** → Lascia di default
3. Clicca **Reimport**

Puoi anche impostarlo globalmente nelle impostazioni di esportazione Web.

## Altri Formati

| Formato       | Uso Principale     | Note                              |
|---------------|--------------------|-----------------------------------|
| Basis Universal | Web (consigliato) | Migliore scelta per HTML5        |
| ETC2          | Mobile + Web      | Buona compressione               |
| ASTC          | Mobile            | Eccellente qualità              |
| S3TC/DXT      | Desktop           | Non ideale per web               |

## Best Practices

### Per il Web
- Usa **Basis Universal** su tutte le texture
- Riduci la risoluzione delle texture quando possibile
- Usa texture atlases (una texture grande invece di tante piccole)
- Evita texture 4K o molto grandi
- Comprime le texture durante l'import

### Quando Aggiungeremo i 7 Gribbits
Quando importeremo modelli 3D per i personaggi:
- Applicheremo Basis Universal su tutte le texture
- Useremo texture a risoluzione ragionevole (512x512 o 1024x1024)
- Eviteremo shader complessi che richiedono texture multiple

## Stato Attuale del Nostro Progetto
Attualmente Gribbits Town usa principalmente mesh primitive senza texture complesse. Questo significa che non abbiamo ancora bisogno di compressione texture pesante.

Quando aggiungeremo modelli 3D dettagliati (i 7 Gribbits), applicheremo queste tecniche.

## Riepilogo
- **Basis Universal** = scelta migliore per web
- Comprime durante l'import in Godot
- Riduci risoluzione quando possibile
- Usa atlases per ottimizzare ulteriormente

*Guida specifica per Gribbits Town*