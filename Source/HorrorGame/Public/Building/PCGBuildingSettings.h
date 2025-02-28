

#pragma once

#include "CoreMinimal.h"
#include "PCGBuildingSettings.generated.h"


class APCGRoom;

UCLASS(BlueprintType, Blueprintable)
class HORRORGAME_API UPCGBuildingSettings : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Transient)
	AActor* ActorOwner = nullptr;

	virtual UWorld* GetWorld() const override;

public:
	void Initialize(AActor* Owner);

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<APCGRoom>, float> RoomsProbabilitySpawn;
};
