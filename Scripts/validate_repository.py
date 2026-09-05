from pathlib import Path
import re
import sys

root = Path(__file__).resolve().parents[1]
errors = []

required = [
    "TheForbiddenGates.uproject",
    "Source/TheForbiddenGates/TheForbiddenGates.Build.cs",
    "Source/TheForbiddenGates/Public/TFGAttributeSet.h",
    "Source/TheForbiddenGates/Public/TFGCharacterBase.h",
    "Source/TheForbiddenGates/Public/TFGPlayerCharacter.h",
    "Source/TheForbiddenGates/Public/TFGEnemyCharacter.h",
    "Source/TheForbiddenGates/Public/TFGProgressionSubsystem.h",
    "Source/TheForbiddenGates/Public/TFGQuestComponent.h",
    "Source/TheForbiddenGates/Public/TFGCheckpointActor.h",
    "Source/TheForbiddenGates/Public/TFGCampaignCatalog.h",
    "Source/TheForbiddenGates/Private/TFGCampaignCatalog.cpp",
    "Source/TheForbiddenGates/Public/TFGCampaignLevelRuntime.h",
    "Source/TheForbiddenGates/Private/TFGCampaignLevelRuntime.cpp",
    "Source/TheForbiddenGates/Public/TFGCampaignEliteEnemy.h",
    "Source/TheForbiddenGates/Private/TFGCampaignEliteEnemy.cpp",
    "Docs/GAME_BIBLE.md",
    "Docs/LEVEL_PLAN_001_100.md",
    "Docs/TECHNICAL_ARCHITECTURE.md",
    "Config/DefaultGameplayTags.ini",
]

for relative in required:
    if not (root / relative).exists():
        errors.append(f"Missing required file: {relative}")

plan_path = root / "Docs/LEVEL_PLAN_001_100.md"
if plan_path.exists():
    plan = plan_path.read_text(encoding="utf-8")
    levels = re.findall(r"(?m)^(\d+)\. \*\*", plan)
    numbers = [int(value) for value in levels]
    if numbers != list(range(1, 101)):
        errors.append(f"Level plan must contain exactly ordered levels 1-100; found {len(numbers)} entries")

catalog_path = root / "Source/TheForbiddenGates/Private/TFGCampaignCatalog.cpp"
if catalog_path.exists():
    catalog = catalog_path.read_text(encoding="utf-8")
    declared_levels = {int(value) for value in re.findall(r"\{(\d+),\s*TEXT\(\"", catalog)}
    expected_levels = set(range(4, 51))
    missing_levels = sorted(expected_levels - declared_levels)
    if missing_levels:
        errors.append(f"Campaign runtime catalog is missing levels: {missing_levels}")
    if 'TEXT("Elyra")' not in catalog or 'LevelNumber == 50' not in catalog:
        errors.append("Level 50 midpoint definition is missing")

game_mode = root / "Source/TheForbiddenGates/Private/TFGGameMode.cpp"
if game_mode.exists():
    text = game_mode.read_text(encoding="utf-8")
    if "FTFGCampaignCatalog::IsRuntimeLevel" not in text or "ATFGCampaignLevelRuntime" not in text:
        errors.append("GameMode must route levels 4-50 through the campaign runtime")

tags_path = root / "Config/DefaultGameplayTags.ini"
if tags_path.exists():
    tags = tags_path.read_text(encoding="utf-8")
    if tags.count('Tag="Realm.') < 10:
        errors.append("Gameplay tags must define all 10 realm tags")
    if tags.count('Tag="Magic.') < 10:
        errors.append("Gameplay tags must define all 10 magic discipline tags")

for banned in ("Binaries", "DerivedDataCache", "Intermediate", "Saved"):
    if (root / banned).exists():
        errors.append(f"Generated Unreal directory should not be tracked: {banned}")

if errors:
    print("Repository validation failed:")
    for error in errors:
        print(f" - {error}")
    sys.exit(1)

print("The Forbidden Gates desktop repository validation passed through campaign level 50.")
