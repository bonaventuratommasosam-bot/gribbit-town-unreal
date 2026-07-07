# Gribbits Town - Unreal Engine Project Structure

## Recommended Folder Structure (Content/)

```
Content/
├── Characters/
│   ├── Blueprints/
│   │   ├── BP_GribbitCharacter.uasset          # Base character Blueprint
│   │   ├── BP_ChillPete.uasset
│   │   ├── BP_SheriffBuck.uasset
│   │   ├── BP_MaxMARFA.uasset
│   │   ├── BP_CaptainFinn.uasset
│   │   ├── BP_ElderMoss.uasset
│   │   ├── BP_LordTiberius.uasset
│   │   ├── BP_LanaMARFA.uasset
│   ├── Meshes/                               # Skeletal Meshes & LODs
│   ├── Animations/
│   ├── Materials/
│   └── Textures/
├── World/
│   ├── Maps/
│   │   ├── GribbitTown_Main.umap
│   │   ├── GribbitTown_Residential.umap
│   │   ├── GribbitTown_Downtown.umap
│   │   └── GribbitTown_Harbor.umap
│   ├── Levels/                               # Sub-levels for streaming
│   ├── Lighting/
│   └── Environment/
├── Systems/
│   ├── Needs/                                # Hunger, Energy, Social, Fun, etc.
│   ├── Character/
│   ├── Interaction/
│   └── AI/
├── UI/
│   ├── Widgets/
│   └── HUD/
├── Data/
│   ├── DataTables/                         # CharacterData, NeedsData, OutfitData
│   └── Curves/
├── VFX/
└── Audio/
```

## C++ Structure (Source/GribbitTown/)

- `GribbitTownGameMode` → Main game rules + town initialization
- `GribbitCharacter` → Base player character (can be subclassed per preset)
- `GribbitNeedsComponent` (new) → Manage needs (Hunger, Energy, Fun, Social...)
- `GribbitOutfitComponent` (new) → Handle outfit swapping
- `GribbitPersonalityComponent` (future) → Personality traits & dialogue

## Naming Conventions

- Blueprints: `BP_`
- Components: `GribbitXXXComponent`
- Data Assets / Data Tables: `DA_` or `DT_`
- Maps: `GribbitTown_XXX`

*Document created with Grok - July 2026*