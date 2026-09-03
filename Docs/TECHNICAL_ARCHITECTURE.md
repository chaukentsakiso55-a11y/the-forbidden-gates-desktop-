# Technical Architecture

## Runtime model
The game is built as a single Unreal Engine 5.8 C++ project with Blueprint-extensible gameplay classes. Systems that must remain identical across platforms are implemented in C++ or data assets; platform differences are isolated to input, rendering, memory budgets, UI layout and optional integrations.

## Core systems
- `TFGGameInstance` — global profile state, save slot selection, campaign metadata and platform capability flags.
- `TFGPlayerCharacter` — movement, camera, interaction hooks and ability-system ownership.
- `TFGPlayerState` — persistent runtime stats, discipline mastery and progression state.
- `TFGAbilitySystemComponent` — gameplay abilities, attributes, effects and tags.
- `TFGSaveGame` — versioned save schema for campaign, settings, choices, collectibles and checkpoints.
- `TFGLevelDefinition` — data asset describing level id, chapter, display name, map, recommended power, checkpoints and unlock rules.
- `TFGQuestDefinition` — data asset for story and optional quest state.
- `TFGMagicDisciplineDefinition` — data asset for discipline metadata and unlock chains.
- `TFGGateSubsystem` — global Gate state, unlocked destinations and narrative seal state.
- `TFGCheckpointSubsystem` — checkpoint registration, respawn and autosave requests.
- `TFGDialogueSubsystem` — dialogue state, speaker metadata, subtitle routing and choice flags.
- `TFGTelemetryInterface` — optional local performance/debug events; no personal data required for core play.

## Gameplay Ability System
Use Unreal Gameplay Ability System for magic, status effects, stamina/resource costs, cooldowns and boss mechanics. Gameplay Tags should describe state instead of hard-coded booleans where practical.

Example tag families:
- `State.Player.*`
- `State.Enemy.*`
- `Magic.Ember.*`
- `Magic.Tide.*`
- `Magic.Frost.*`
- `Magic.Storm.*`
- `Magic.Verdant.*`
- `Magic.Radiant.*`
- `Magic.Umbral.*`
- `Magic.Spirit.*`
- `Magic.Rune.*`
- `Magic.Gatefire.*`
- `World.Gate.*`
- `Quest.Main.*`
- `Quest.Optional.*`

## Level streaming
Each numbered level is a logical mission containing:
1. persistent level shell;
2. streamed environment cells;
3. encounter and puzzle layers;
4. cinematic layer;
5. platform-specific visual override layer when required.

World Partition may be used for larger levels, while smaller dungeon interiors can remain conventional maps. Avoid making the entire 100-level campaign one continuous world.

## Save schema
Every save stores a `SaveVersion` integer. Migrations must preserve older campaign saves after patches.

Required data:
- profile id and display settings;
- current level and checkpoint;
- completed levels;
- unlocked Gate destinations;
- discipline mastery;
- relic and collectible ids;
- quest states;
- important narrative choices;
- accessibility settings;
- input mappings;
- total play time;
- New Game+ state.

## Checkpoint policy
Long levels autosave at authored checkpoints, before major story sequences, after major encounters, and when entering safe hubs. Autosaves should be atomic: write to a temporary slot then promote only after successful serialization.

## AI architecture
Enemy behavior uses Behavior Trees/StateTree where appropriate. Boss logic should be modular and driven by encounter phases rather than one giant Blueprint graph.

## UI
Use CommonUI with platform-aware input glyphs. Desktop supports keyboard/mouse and controller. Android uses a separate touch layout that maps into the same Enhanced Input actions.

## Rendering tiers
### Desktop reference
- High-quality lighting and shadows
- Niagara-heavy magic VFX where supported
- Higher view distance and foliage density
- High resolution textures
- Optional high-end features exposed through settings

### Android
- Device profile based scalability
- Reduced dynamic-light count
- Simplified particles and translucent layers
- Lower texture groups and streaming pool
- Reduced foliage and distant detail
- Baked or cheaper lighting where it materially improves performance
- Thermal-aware frame targets and quality presets

## Performance budgets
Budgets should be enforced during content production rather than only at the end.

Desktop baseline target: stable 60 fps on the defined minimum PC specification at the selected baseline quality preset.
Android target: stable 30 fps on supported baseline hardware, with 45/60 fps options only on devices that can sustain them thermally.

## Content pipeline
1. Concept approved in the game bible.
2. Greybox created and playtested.
3. Gameplay mechanics validated.
4. Environment art pass.
5. Lighting/VFX/audio pass.
6. Optimization pass.
7. Accessibility and UI pass.
8. QA regression.
9. Release branch validation.

## Source-control rules
- Never commit DerivedDataCache, Intermediate, Saved or packaged builds.
- Large binary Unreal assets should use Git LFS when available.
- Keep canonical narrative and level IDs stable once public builds exist.
- Production branches should require successful build validation before release tagging.

## Security and online scope
The base game is single-player and does not require privileged device access. If cloud saves, accounts or online features are added later, credentials must never be embedded in the client repository.

## Release gates
A release candidate cannot be called production-ready until:
- all 100 level maps load and complete without blockers;
- all critical paths have save/restore coverage;
- no known crash-level defects remain;
- input is validated on each supported platform;
- memory and performance budgets pass on minimum devices;
- accessibility settings persist correctly;
- Android package meets current Play requirements;
- desktop installer/package passes clean-machine testing;
- credits, licenses and asset permissions are complete.
