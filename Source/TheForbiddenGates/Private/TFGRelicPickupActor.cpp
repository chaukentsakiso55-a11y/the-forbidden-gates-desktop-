#include "TFGRelicPickupActor.h"
#include "Engine/GameInstance.h"
#include "TFGPlayerCharacter.h"
#include "TFGProgressionSubsystem.h"

ATFGRelicPickupActor::ATFGRelicPickupActor(){InteractionPrompt=FText::FromString(TEXT("Collect relic"));SpeakerName=FText::FromString(TEXT("Relic"));bOneShot=true;}
bool ATFGRelicPickupActor::CanInteract_Implementation(APawn* Interactor) const
{
    if(!Super::CanInteract_Implementation(Interactor))return false;
    if(!RelicId.IsNone())
    {
        if(UGameInstance* GI=GetGameInstance())
        {
            if(UTFGProgressionSubsystem* P=GI->GetSubsystem<UTFGProgressionSubsystem>())
            {
                const bool bAlreadyCollected=P->HasRelic(RelicId);
                const bool bQuestLinkedRecovery=!QuestId.IsNone()&&RequiredQuestStage>=0;
                if(bAlreadyCollected&&!bQuestLinkedRecovery)return false;
            }
        }
    }
    return true;
}
void ATFGRelicPickupActor::Interact_Implementation(APawn* Interactor)
{
    if(!CanInteract_Implementation(Interactor))return;
    if(UGameInstance* GI=GetGameInstance())
    {
        if(UTFGProgressionSubsystem* P=GI->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if(!RelicId.IsNone())P->CollectRelic(RelicId);
            if(!ItemId.IsNone()&&Quantity>0)P->AddItem(ItemId,Quantity);
            if(!DisciplineId.IsNone()&&MasteryAmount>0)P->AddDisciplineMastery(DisciplineId,MasteryAmount);
            if(!AbilityUnlockId.IsNone())P->UnlockAbility(AbilityUnlockId);
        }
    }
    if(ATFGPlayerCharacter* Player=Cast<ATFGPlayerCharacter>(Interactor))Player->RefreshUnlockedAbilities();
    Super::Interact_Implementation(Interactor);
}
