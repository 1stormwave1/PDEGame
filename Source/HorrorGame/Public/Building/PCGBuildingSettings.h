

#pragma once

#include "CoreMinimal.h"
#include "PCGSettings.generated.h"


class ARoom;

UCLASS(BlueprintType, Blueprintable)
class HORRORGAME_API UPCGSettings : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Transient)
	AActor* ActorOwner = nullptr;

	virtual UWorld* GetWorld() const override;

public:
	void Initialize(AActor* Owner);

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<ARoom>, float> RoomsProbabilitySpawn;
};
