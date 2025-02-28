

#pragma once

#include "CoreMinimal.h"
#include "PCGNode.generated.h"


class ARoom;

UCLASS(BlueprintType, Blueprintable)
class HORRORGAME_API UPCGNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPCGNode*> ChildNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARoom> RoomClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRoot = false;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsLeaf() const { return ChildNodes.IsEmpty(); }
};
