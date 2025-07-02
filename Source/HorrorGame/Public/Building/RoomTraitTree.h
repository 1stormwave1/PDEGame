

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.generated.h"

UENUM(BlueprintType)
enum class ETraitEnum : uint8
{
	None,
	Treasure,
	Dialogue,
	Enemy
};

UENUM(BlueprintType)
enum class ERoomTypeEnum : uint8
{
	None,
	Start,
	Finish,
	Transition
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomTypeEnum RoomType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTTNode> Traits;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeByRoomType(ERoomTypeEnum NewRoomType);
};
