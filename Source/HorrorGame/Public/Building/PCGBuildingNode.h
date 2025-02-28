

#pragma once

#include "CoreMinimal.h"
#include "PCGBuildingNode.generated.h"




UCLASS(BlueprintType, Blueprintable)
class HORRORGAME_API UPCGBuildingNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPCGBuildingNode*> ChildNodes;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRoot = false;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsLeaf() const { return ChildNodes.IsEmpty(); }
};
