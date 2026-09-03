# The Forbidden Gates — Desktop

A cinematic third-person fantasy action-adventure RPG built with Unreal Engine 5.8.

## Vision
The kingdom of Elaris is attacked by the Veyr Dominion, and Princess Elyra Vale disappears beyond the ancient Forbidden Gates. The player follows her across ten magical realms and one hundred story levels, uncovering the true purpose of the Gates, the Gate Masters, the Aetherians, and the entity known as the First Hollow.

## Production targets
- Engine: Unreal Engine 5.8
- Primary platform: Windows desktop
- Gameplay: third-person exploration, magic combat, traversal, puzzles, bosses, quests, collectibles and cinematic story scenes
- Structure: 10 chapters × 10 long levels
- Architecture: data-driven gameplay systems with platform-specific scalability profiles
- Save system: local profiles, checkpoints, chapter progression, settings, collectibles and choices
- Accessibility: subtitles, remappable controls, scalable UI, camera sensitivity, color-independent puzzle cues and difficulty assists

## Repository layout
- `TheForbiddenGates.uproject` — Unreal project descriptor
- `Source/` — C++ game framework
- `Config/` — engine and platform configuration
- `Content/` — Unreal assets and maps (added through Unreal Editor)
- `Docs/` — game design, story, level plan, technical design and production checklist

## Canonical game pillars
1. **Wonder** — every realm should feel magical and visually distinct.
2. **Discovery** — the story grows from a rescue mission into an ancient-world mystery.
3. **Mastery** — the player learns a flexible magic system rather than only gaining larger numbers.
4. **Journey** — the hundred-level campaign should feel like travelling far beyond the known world.
5. **Choice** — late-game decisions affect relationships and the fate of the Gates.

## Build philosophy
Desktop is the reference-quality version. Android shares the same narrative, systems and content design, but uses separate rendering, memory and control profiles.

## Status
Production foundation initialized. Art assets, Unreal maps, animation, audio, VFX, balancing and final packaged builds still require editor-side production and QA.
