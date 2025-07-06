

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.h"
#include "GameFramework/SaveGame.h"
#include "GeneticAlgorithmSave.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FSavedRoomTraits
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTTNode> RoomTraits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomTypeEnum RoomType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime SaveTime;
};


UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UGeneticAlgorithmSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FSavedRoomTraits> SavedRoomTraits;

	UFUNCTION(BlueprintCallable)
	void GetTraitsByRoomType(TArray<FSavedRoomTraits>& OutTraits, ERoomTypeEnum RoomType);

	UFUNCTION(BlueprintCallable)
	void AddRoomTraitsToSave(URoomTraits* RoomTraits);
	
};
