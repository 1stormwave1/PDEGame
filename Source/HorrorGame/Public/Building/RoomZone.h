
#pragma once

#include "CoreMinimal.h"
#include "PCGRoom.h"
#include "GameFramework/Actor.h"
#include "RoomZone.generated.h"

class UBoxComponent;

UCLASS()
class HORRORGAME_API ARoomZone : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> TriggerBox;
	
	UPROPERTY(Transient)
    TObjectPtr<APCGRoom> RoomOwner = nullptr;	
	
public:
	UPROPERTY(EditAnywhere)
	ERoomType Type;

	ARoomZone();

	UFUNCTION(BlueprintNativeEvent)
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
};
