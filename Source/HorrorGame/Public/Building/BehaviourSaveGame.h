

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BehaviourAnalysis.h"
#include "BehaviourSaveGame.generated.h"


UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UBehaviourSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FBehaviourVector> BehaviourData;

	UFUNCTION(BlueprintCallable)
	void WriteJson(FString JsonFilePath);
};
