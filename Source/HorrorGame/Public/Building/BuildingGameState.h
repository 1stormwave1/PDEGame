

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BuildingGameState.generated.h"


class APCGRoom;
class UPCGNode;
class UPCGAlgorithm;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentRoomChangedSignature, APCGRoom*, NewCurrentRoom);

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
	void SpawnRooms(UPCGBuildingNode* Root);

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	APCGRoom* CurrentRoom = nullptr;
	
	UPROPERTY(BlueprintAssignable)
	FOnCurrentRoomChangedSignature OnCurrentRoomChangedDelegate;

	UFUNCTION(BlueprintNativeEvent)
	void OnCurrentRoomChanged(APCGRoom* NewRoom);
};
