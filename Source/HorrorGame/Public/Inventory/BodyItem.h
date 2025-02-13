

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
#include "BodyItem.generated.h"

UENUM(BlueprintType)
enum class EBodyType : uint8
{
	None,
	Brain,
	EyeBalls,
	Lungs,
	Heart,
	Spine
};

UCLASS()
class HORRORGAME_API UBodyItem : public UItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBodyType BodyType;
};
