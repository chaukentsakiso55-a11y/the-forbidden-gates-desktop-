# The Forbidden Gates — Implementation Status

## Current production slice

The desktop and Android projects share the same gameplay architecture and canonical campaign order.

### Playable code-driven campaign prototypes

1. **Level 1 — Morning in Elaris**
   - Captain Arlen training sequence
   - Festival market and Mira interaction
   - Princess Elyra festival encounter
   - City overlook transition
   - Checkpoints and persistent quest state

2. **Level 2 — The Sky Turns Red**
   - Warning-bell opening
   - Civilian evacuation route
   - First Veyr encounters
   - Southern-district rescue sequence
   - Veyr blockade with the first Brute archetype
   - Protected-district transition

3. **Level 3 — Palace Under Siege**
   - Breached royal avenue
   - Palace approach encounters
   - Aetherian Gate shard relic
   - Gatefire Echo unlock
   - Heavy palace-step assault
   - Defensive ward activation
   - Royal hall objective

## Shared gameplay systems

- Gameplay Ability System foundation
- Health, mana, stamina, magic power, and armor
- Arcane Bolt primary spell
- Gatefire Echo secondary spell
- Mana cost and cooldown enforcement
- Dodge with stamina cost and resource regeneration
- Enemy sight/chase/melee behavior
- Veyr Brute enemy archetype
- Quest persistence
- Checkpoints and save restoration
- Interaction and dialogue system
- Objective HUD and combat HUD
- Encounter zones
- Relic collection, inventory, ability unlocks, and discipline mastery
- Code-built placeholder environments for gameplay testing before final art maps

## Platform-specific layers

Android additionally contains its mobile touch controls, device-quality logic, thermal safeguards, and first-launch Android TTS narration bridge. Desktop keeps keyboard/mouse and controller controls and Windows packaging workflows.

## Validation status

Repository-health workflows validate required project structure and production rules. They are not a substitute for a full Unreal Engine C++ compile, cook, package, or device test. Final Windows and Android binaries still require an Unreal Engine 5.8.x build machine with the necessary platform toolchains.

## Next campaign target

**Level 4 — The Vanishing Princess**

Planned production slice:
- inner royal hall
- Elyra disappearance sequence
- Veyr commander encounter setup
- first evidence that the attackers came specifically for Elyra
- additional relic/loot opportunities
- transition toward Level 5 — Ashes of the Courtyard
