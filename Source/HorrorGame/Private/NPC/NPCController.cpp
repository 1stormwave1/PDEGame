


#include "NPC/NPCController.h"
#include "Base/Components/DialogueComponent.h"

ANPCController::ANPCController()
{
	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
}
