

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
#include "FillUpItem.generated.h"


UCLASS()
class HORRORGAME_API UFillUpItem : public UItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FillValue = 0.5f;
};
