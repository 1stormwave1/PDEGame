

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MainSaveGame.generated.h"


UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 GlobalItemCollected = 0;

	UPROPERTY(EditAnywhere)
	int32 BuildingItemsCollected = 0;

	UPROPERTY(EditAnywhere)
	int32 MainStoryStep = 0;
	
};
