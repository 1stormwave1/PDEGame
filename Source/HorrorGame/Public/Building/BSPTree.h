

#pragma once

#include "CoreMinimal.h"
#include "BSPTree.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FBSPNode
{
	GENERATED_BODY()

	
};


UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API UBSPTree : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FBSPNode> Tree;
	
	UFUNCTION(BlueprintCallable)
	void GetLeaves(TArray<FBSPNode>& OutLeaves);
};
