#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TFGSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FTFGQuestState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName QuestId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Stage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompleted = false;
};

UCLASS(BlueprintType)
class THEFORBIDDENGATES_API UTFGSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 SaveVersion = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 CurrentLevel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName CurrentCheckpoint = NAME_None;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSet<int32> CompletedLevels;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSet<FName> UnlockedGates;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<FName, int32> DisciplineMastery;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSet<FName> CollectedRelics;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FTFGQuestState> QuestStates;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<FName, int32> NarrativeChoices;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TotalPlayTimeSeconds = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool bNewGamePlus = false;
};
