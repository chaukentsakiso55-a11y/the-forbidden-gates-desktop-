# The Forbidden Gates — Content Production Pipeline

This phase turns the code-complete 100-level prototype campaign into a finished game without changing campaign numbering or save compatibility.

## Production rule

Gameplay code, campaign state and final assets are separate layers. A level may be code-playable before final art exists, but it is not release-ready until its production checklist passes.

## Per-level asset contract

`TFGLevelContentProfile` assigns every level:

- environment theme
- lighting mood
- weather state
- music state
- traversal mechanic
- puzzle archetype
- optional intro/outro cinematic IDs
- optional named boss ID
- canonical art, audio and VFX content folders

Expected Unreal content roots:

- `/Game/Production/Worlds/Chapter_XX/Level_XXX`
- `/Game/Production/Audio/Chapter_XX/Level_XXX`
- `/Game/Production/VFX/Chapter_XX/Level_XXX`

Final assets can replace code-generated placeholder geometry without changing quest IDs, map IDs, save data or level numbering.

## Level completion gates

A production level is complete only when all applicable gates pass:

1. Gameplay — critical path, optional exploration, encounters, checkpoints and fail/retry behavior work.
2. Environment — final modular kit, landmarks, collision, traversal readability and level dressing are authored.
3. Lighting — level mood, exposure, readability and performance targets are validated.
4. Characters — required hero/NPC/enemy models, rigs, animation sets and materials are release-approved.
5. VFX — magic, Gate effects, hazards, impacts and boss phase visuals have quality tiers.
6. Audio — music state, ambience, interaction sounds, combat audio and dialogue/voice requirements are hooked up.
7. Cinematics — required story sequences are authored, subtitle-safe and skippable where appropriate.
8. Accessibility — subtitles, objective readability, input parity and effects readability are checked.
9. Performance — CPU/GPU/memory budgets pass on Windows target specs and Android quality tiers.
10. QA — clean-start, checkpoint restore, save migration, death/retry, transition and campaign progression tests pass.

## Hero levels

Hero levels receive bespoke art/cinematic attention before ordinary levels:

1, 4, 10, 19, 20, 29, 39, 40, 50, 59, 69, 74, 77, 83, 84, 90, 94, 98, 99 and 100.

## Boss production contract

Named bosses are represented by `ATFGCampaignEliteEnemy` with campaign-specific profiles and three runtime health phases. Boss Blueprints can bind to `OnBossPhaseChanged` to drive phase-specific animations, abilities, arena changes, music intensity, lighting/VFX shifts, dialogue and camera emphasis.

Named production bosses currently defined:

- Level 10 — Dominion Commander
- Level 19 — Guardian of Roots
- Level 29 — Seraphon, Master of Flame
- Level 39 — The Drowned King
- Level 69 — Vael the Fallen
- Level 99 — The First Hollow

## Platform production rule

Desktop is the reference-quality visual target. Android keeps the same campaign, mechanics and save contract but uses scalable assets, LODs, particle counts, shadows, streaming ranges and compressed audio tiers.

No platform optimization may alter story choices, quest IDs, reward IDs or save compatibility.

## Current next target

Build the first final-quality vertical slice around Levels 1–10, with Level 10 as the first production boss benchmark. Once it passes the release gates, scale the same workflow chapter by chapter through Level 100.
