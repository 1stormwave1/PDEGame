

#pragma once

#include "CoreMinimal.h"
#include "PCGTree.generated.h"


class UPCGNode;

UCLASS(BlueprintType, Blueprintable)
class HORRORGAME_API UPCGTree : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	UPCGNode* Root = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPCGNode> NodeClass;

	void Initialize();
};
