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
    "Source/TheForbiddenGates/Public/TFGLevelContentProfile.h",
    "Source/TheForbiddenGates/Private/TFGLevelContentProfile.cpp",
    "Source/TheForbiddenGates/Public/TFGContentBlueprintLibrary.h",
    "Source/TheForbiddenGates/Private/TFGContentBlueprintLibrary.cpp",
    "Source/TheForbiddenGates/Public/TFGProductionAtmosphere.h",
    "Source/TheForbiddenGates/Private/TFGProductionAtmosphere.cpp",
    "Source/TheForbiddenGates/Public/TFGEndingChoiceActor.h",
    "Source/TheForbiddenGates/Private/TFGEndingChoiceActor.cpp",
    "Docs/GAME_BIBLE.md",
    "Docs/LEVEL_PLAN_001_100.md",
    "Docs/TECHNICAL_ARCHITECTURE.md",
    "Docs/CONTENT_PRODUCTION_PIPELINE.md",
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
    expected_levels = set(range(4, 101))
    missing_levels = sorted(expected_levels - declared_levels)
    if missing_levels:
        errors.append(f"Campaign runtime catalog is missing levels: {missing_levels}")
    if 'TEXT("Elyra")' not in catalog or 'LevelNumber==50' not in catalog.replace(" ", ""):
        errors.append("Level 50 midpoint definition is missing")
    if 'TEXT("What Lies Beyond")' not in catalog or 'LevelNumber==100' not in catalog.replace(" ", ""):
        errors.append("Level 100 finale definition is missing")

header = root / "Source/TheForbiddenGates/Public/TFGCampaignCatalog.h"
if header.exists():
    text = header.read_text(encoding="utf-8")
    if "LevelNumber <= 100" not in text:
        errors.append("Campaign runtime must accept levels through 100")
    if "bFinalLevel" not in text:
        errors.append("Campaign catalog must mark the final level")

runtime_path = root / "Source/TheForbiddenGates/Private/TFGCampaignLevelRuntime.cpp"
if runtime_path.exists():
    text = runtime_path.read_text(encoding="utf-8")
    for token in ("ATFGEndingChoiceActor", "Seal the Gates", "Destroy the Gates", "Control the Gates", "bNewGamePlus", "CampaignComplete"):
        if token not in text:
            errors.append(f"Finale runtime is missing required token: {token}")
    if "Save->CurrentLevel = 51" not in text:
        errors.append("Old Level 50 midpoint saves must migrate into Level 51")

game_mode = root / "Source/TheForbiddenGates/Private/TFGGameMode.cpp"
if game_mode.exists():
    text = game_mode.read_text(encoding="utf-8")
    if "FTFGCampaignCatalog::IsRuntimeLevel" not in text or "ATFGCampaignLevelRuntime" not in text:
        errors.append("GameMode must route runtime campaign levels through ATFGCampaignLevelRuntime")
    if "ATFGProductionAtmosphere" not in text:
        errors.append("GameMode must spawn the production atmosphere director")

content_profile = root / "Source/TheForbiddenGates/Private/TFGLevelContentProfile.cpp"
if content_profile.exists():
    content_text = content_profile.read_text(encoding="utf-8")
    for token in (
        "Elaris_FallenRoyal", "WhisperingWilds_Bioluminescent", "EmberKingdom_VolcanicForge",
        "SunkenRealm_DrownedAetherian", "Stormlands_FloatingCitadels", "BrokenKingdom_FracturedVeyr",
        "ShadowRealm_UmbralRuins", "AncientWorld_AetherianMachineGarden", "LastGate_WarCrossroads",
        "HeartGate_ElarisTransformed", "CIN_FinalChoice", "FirstHollow"
    ):
        if token not in content_text:
            errors.append(f"Production content profile is missing required token: {token}")

profile_header = root / "Source/TheForbiddenGates/Public/TFGLevelContentProfile.h"
if profile_header.exists():
    profile_text = profile_header.read_text(encoding="utf-8")
    for token in ("USTRUCT(BlueprintType)", "EnvironmentTheme", "MusicState", "IntroCinematicId", "BossId"):
        if token not in profile_text:
            errors.append(f"Blueprint content profile is missing required token: {token}")

blueprint_library = root / "Source/TheForbiddenGates/Public/TFGContentBlueprintLibrary.h"
if blueprint_library.exists():
    library_text = blueprint_library.read_text(encoding="utf-8")
    if "GetLevelContentProfile" not in library_text or "BlueprintPure" not in library_text:
        errors.append("Production content catalog must be accessible from Blueprints")

atmosphere = root / "Source/TheForbiddenGates/Private/TFGProductionAtmosphere.cpp"
if atmosphere.exists():
    atmosphere_text = atmosphere.read_text(encoding="utf-8")
    for token in ("UDirectionalLightComponent", "UExponentialHeightFogComponent", "Profile.ChapterNumber", "Profile.bHeroLevel"):
        if token not in atmosphere_text:
            errors.append(f"Production atmosphere is missing required token: {token}")

boss = root / "Source/TheForbiddenGates/Private/TFGCampaignEliteEnemy.cpp"
if boss.exists():
    boss_text = boss.read_text(encoding="utf-8")
    for token in ("DominionCommander", "GuardianOfRoots", "Seraphon", "DrownedKing", "VaelTheFallen", "FirstHollow", "UpdateBossPhase"):
        if token not in boss_text:
            errors.append(f"Named boss production layer is missing required token: {token}")

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

print("The Forbidden Gates desktop repository validation passed through campaign level 100 and production content phase 1.")
