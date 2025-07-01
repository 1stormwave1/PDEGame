

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.generated.h"

UENUM()
enum class ETraitEnum : uint8
{
	None,
	Treasure,
	Dialogue,
	Enemy
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
class HORRORGAME_API URoomTraits : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FTTNode> Traits;
};
