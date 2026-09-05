#include "TFGGameMode.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TFGLevelOneDirector.h"
#include "TFGLevelOnePrototypeWorld.h"
#include "TFGLevelThreeDirector.h"
#include "TFGLevelThreePrototypeWorld.h"
#include "TFGLevelTwoDirector.h"
#include "TFGLevelTwoPrototypeWorld.h"
#include "TFGPlayerCharacter.h"
#include "TFGProgressionSubsystem.h"
#include "TFGSaveGame.h"

ATFGGameMode::ATFGGameMode(){DefaultPawnClass=ATFGPlayerCharacter::StaticClass();}
void ATFGGameMode::BeginPlay()
{
    Super::BeginPlay();
    UGameInstance* GI=GetGameInstance(); if(!GI||!GetWorld())return;
    UTFGProgressionSubsystem* P=GI->GetSubsystem<UTFGProgressionSubsystem>(); UTFGSaveGame* S=P?P->GetCurrentSave():nullptr; if(!S)return;
    FActorSpawnParameters Params;Params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;FName Expected=NAME_None;const FVector Spawn(0,0,140);
    if(S->CurrentLevel==1&&!S->CompletedLevels.Contains(1)){Expected=TEXT("L01_MorningInElaris");GetWorld()->SpawnActor<ATFGLevelOneDirector>(ATFGLevelOneDirector::StaticClass(),FVector::ZeroVector,FRotator::ZeroRotator,Params);GetWorld()->SpawnActor<ATFGLevelOnePrototypeWorld>(ATFGLevelOnePrototypeWorld::StaticClass(),FVector::ZeroVector,FRotator::ZeroRotator,Params);}else if(S->CurrentLevel==2&&!S->CompletedLevels.Contains(2)){Expected=TEXT("L02_TheSkyTurnsRed");GetWorld()->SpawnActor<ATFGLevelTwoDirector>(ATFGLevelTwoDirector::StaticClass(),FVector::ZeroVector,FRotator::ZeroRotator,Params);GetWorld()->SpawnActor<ATFGLevelTwoPrototypeWorld>(ATFGLevelTwoPrototypeWorld::StaticClass(),FVector::ZeroVector,FRotator::ZeroRotator,Params);}else if(S->CurrentLevel==3&&!S->CompletedLevels.Contains(3)){Expected=TEXT("L03_PalaceUnderSiege");GetWorld()->SpawnActor<ATFGLevelThreeDirector>(ATFGLevelThreeDirector::StaticClass(),FVector::ZeroVector,FRotator::ZeroRotator,Params);GetWorld()->SpawnActor<ATFGLevelThreePrototypeWorld>(ATFGLevelThreePrototypeWorld::StaticClass(),FVector::ZeroVector,FRotator::ZeroRotator,Params);}
    if(APawn* Pawn=UGameplayStatics::GetPlayerPawn(this,0)){if(!Expected.IsNone()&&S->LastMapId==Expected&&!S->CurrentCheckpoint.IsNone())Pawn->SetActorTransform(S->LastPlayerTransform,false,nullptr,ETeleportType::TeleportPhysics);else Pawn->SetActorLocationAndRotation(Spawn,FRotator::ZeroRotator,false,nullptr,ETeleportType::TeleportPhysics);}
}
void ATFGGameMode::CompleteCurrentLevel(float T){if(UGameInstance* GI=GetGameInstance()){if(UTFGProgressionSubsystem* P=GI->GetSubsystem<UTFGProgressionSubsystem>()){if(UTFGSaveGame* S=P->GetCurrentSave())P->CompleteLevel(S->CurrentLevel,T);}}}
