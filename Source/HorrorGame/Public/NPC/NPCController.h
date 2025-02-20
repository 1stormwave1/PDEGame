

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

class UDialogueComponent;

UCLASS()
class HORRORGAME_API ANPCController : public AAIController
{
	GENERATED_BODY()

public:
	ANPCController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDialogueComponent> DialogueComponent;
};
