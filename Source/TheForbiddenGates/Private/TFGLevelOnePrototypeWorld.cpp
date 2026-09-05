#include "TFGLevelOnePrototypeWorld.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TFGCheckpointActor.h"
#include "TFGEncounterZone.h"
#include "TFGEnemyCharacter.h"
#include "TFGInteractableActor.h"
#include "TFGLevelTransitionActor.h"

ATFGLevelOnePrototypeWorld::ATFGLevelOnePrototypeWorld()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")); SetRootComponent(SceneRoot);
    Geometry = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PrototypeGeometry")); Geometry->SetupAttachment(SceneRoot); Geometry->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); Geometry->SetCollisionResponseToAllChannels(ECR_Block);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube")); if (CubeMesh.Succeeded()) Geometry->SetStaticMesh(CubeMesh.Object);
}

void ATFGLevelOnePrototypeWorld::BeginPlay()
{
    Super::BeginPlay();
    AddBox(FVector(1800,0,-70), FVector(40,10,0.7)); AddBox(FVector(1800,1050,250), FVector(40,0.5,3.2)); AddBox(FVector(1800,-1050,250), FVector(40,0.5,3.2));
    for (int32 Index=0; Index<7; ++Index) { const float X=450+Index*480; AddBox(FVector(X,650,150), FVector(1.8,2.2,3.0)); AddBox(FVector(X+160,-650,130), FVector(1.5,2.0,2.6)); }
    AddBox(FVector(2450,0,120), FVector(4,3,2.4)); AddBox(FVector(3300,0,180), FVector(5,5,3.6));
    auto SpawnCheckpoint=[this](const FVector& Location,FName Id){ FActorSpawnParameters P; P.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn; if(ATFGCheckpointActor* C=GetWorld()->SpawnActor<ATFGCheckpointActor>(ATFGCheckpointActor::StaticClass(),Location,FRotator::ZeroRotator,P)) C->ConfigureCheckpoint(Id,TEXT("L01_MorningInElaris"),true); };
    SpawnCheckpoint(FVector(200,0,90),TEXT("L01_Start"));
    if (ATFGInteractableActor* A=SpawnInteraction(FVector(520,0,70),FText::FromString(TEXT("Speak to Captain Arlen")),TEXT("CaptainArlen"),0,1)) { A->SpeakerName=FText::FromString(TEXT("Captain Arlen")); A->StoryLine=FText::FromString(TEXT("Festival duty or not, your stance still matters. Show me what you remember.")); }
    SpawnEncounter(FVector(1020,0,40),1,2,2);
    if (ATFGInteractableActor* A=SpawnInteraction(FVector(1600,80,70),FText::FromString(TEXT("Speak with Mira at the festival market")),TEXT("MiraFestival"),2,3)) { A->SpeakerName=FText::FromString(TEXT("Mira")); A->StoryLine=FText::FromString(TEXT("Everyone is pretending the border rumors are nothing. I hope they are right.")); }
    SpawnCheckpoint(FVector(2050,0,90),TEXT("L01_FestivalMarket"));
    if (ATFGInteractableActor* A=SpawnInteraction(FVector(2500,0,90),FText::FromString(TEXT("Meet Princess Elyra")),TEXT("PrincessElyraFestival"),3,4)) { A->SpeakerName=FText::FromString(TEXT("Princess Elyra")); A->StoryLine=FText::FromString(TEXT("Kael, look at the old Gate carvings on the festival banners. Strange that we celebrate symbols nobody can explain.")); }
    FActorSpawnParameters P; P.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (ATFGLevelTransitionActor* Exit=GetWorld()->SpawnActor<ATFGLevelTransitionActor>(ATFGLevelTransitionActor::StaticClass(),FVector(3300,0,100),FRotator::ZeroRotator,P)) { Exit->InteractionId=TEXT("FestivalOverlook"); Exit->InteractionPrompt=FText::FromString(TEXT("Walk to the city overlook")); Exit->QuestId=QuestId; Exit->RequiredQuestStage=4; Exit->AdvanceQuestToStage=5; Exit->bCompleteQuest=true; Exit->bOneShot=true; Exit->LevelToComplete=1; Exit->SpeakerName=FText::FromString(TEXT("Kael")); Exit->StoryLine=FText::FromString(TEXT("The bells roll across Elaris. Below, the festival shines as though nothing in the world could reach it.")); Exit->RefreshPresentation(); }
}
void ATFGLevelOnePrototypeWorld::AddBox(const FVector& L,const FVector& S,const FRotator& R){ if(Geometry) Geometry->AddInstance(FTransform(R,L,S)); }
ATFGInteractableActor* ATFGLevelOnePrototypeWorld::SpawnInteraction(const FVector& L,const FText& Prompt,FName Id,int32 Req,int32 Adv){ if(!GetWorld())return nullptr; FActorSpawnParameters P; P.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; ATFGInteractableActor* A=GetWorld()->SpawnActor<ATFGInteractableActor>(ATFGInteractableActor::StaticClass(),L,FRotator::ZeroRotator,P); if(A){A->InteractionPrompt=Prompt;A->InteractionId=Id;A->QuestId=QuestId;A->RequiredQuestStage=Req;A->AdvanceQuestToStage=Adv;A->bOneShot=true;A->RefreshPresentation();} return A; }
ATFGEncounterZone* ATFGLevelOnePrototypeWorld::SpawnEncounter(const FVector& L,int32 Req,int32 Adv,int32 Count){ if(!GetWorld())return nullptr; FActorSpawnParameters P;P.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;ATFGEncounterZone* E=GetWorld()->SpawnActor<ATFGEncounterZone>(ATFGEncounterZone::StaticClass(),L,FRotator::ZeroRotator,P);if(E){E->QuestId=QuestId;E->RequiredQuestStage=Req;E->AdvanceQuestToStage=Adv;for(int32 I=0;I<Count;++I)E->EnemyClasses.Add(ATFGEnemyCharacter::StaticClass());}return E; }
