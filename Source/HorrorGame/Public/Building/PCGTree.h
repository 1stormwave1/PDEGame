

#pragma once

#include "CoreMinimal.h"
#include "PCGTree.generated.h"


class UPCGBuildingNode;

UCLASS(BlueprintType, Blueprintable)
class HORRORGAME_API UPCGTree : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	UPCGBuildingNode* Root = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPCGBuildingNode> NodeClass;

	void Initialize();
};
