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

print("The Forbidden Gates repository validation passed.")
