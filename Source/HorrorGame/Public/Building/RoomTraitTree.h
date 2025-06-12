

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.generated.h"

UENUM()
enum class ETraitEnum : uint8
{
	None,
	Start,
	Finish,
	Transition,
	Treasure,
	Dialogue
};


USTRUCT(BlueprintType, Blueprintable)
struct FTTNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETraitEnum Trait = ETraitEnum::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.f;

	FTTNode(ETraitEnum NewTrait);

	FTTNode();
};

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API URoomTraitTree : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FTTNode> Tree;
};
