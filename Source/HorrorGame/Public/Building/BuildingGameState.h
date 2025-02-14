

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BuildingGameState.generated.h"


class UPCGNode;
class UPCGAlgorithm;

UCLASS()
class HORRORGAME_API ABuildingGameState : public AGameStateBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPCGAlgorithm> PCGAlgorithmClass;

protected:
	UPROPERTY(Transient, BlueprintReadWrite)
	UPCGAlgorithm* PCGAlgorithm = nullptr;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SpawnRooms(UPCGNode* Root);
};
