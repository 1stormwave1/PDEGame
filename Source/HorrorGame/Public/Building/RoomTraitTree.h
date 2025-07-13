

#pragma once

#include "CoreMinimal.h"
#include "RoomTraitTree.generated.h"

UENUM(BlueprintType)
enum class ETraitEnum : uint8
{
	None,
	Treasure,
	Dialogue,
	Enemy,
	Trap,
	Light
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

	friend bool operator==(const FTTNode& Lhs, const FTTNode& RHS)
	{
		return Lhs.Trait == RHS.Trait
			&& Lhs.Weight == RHS.Weight;
	}

	friend bool operator!=(const FTTNode& Lhs, const FTTNode& RHS)
	{
		return !(Lhs == RHS);
	}

	FTTNode(const FTTNode& Other)
		: Trait(Other.Trait),
		  Weight(Other.Weight)
	{
	}

	FTTNode& operator=(const FTTNode& Other)
	{
		if (this == &Other)
			return *this;
		Trait = Other.Trait;
		Weight = Other.Weight;
		return *this;
	}

	FTTNode(ETraitEnum NewTrait);

	FTTNode();
};

USTRUCT(BlueprintType, Blueprintable)
struct FTraitSpawnChance
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTTNode Node;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance = 1.f;
};

USTRUCT(Blueprintable, BlueprintType)
struct FTraitsData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETraitEnum Trait = ETraitEnum::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AActor>, float> ActorClassSpawnChance;
	
};

UCLASS(Blueprintable, BlueprintType)
class HORRORGAME_API URoomTraits : public UObject
{
	GENERATED_BODY()

	inline static const TArray<ETraitEnum> TraitEnums = {
		ETraitEnum::Treasure,
		ETraitEnum::Dialogue,
		ETraitEnum::Enemy,
		ETraitEnum::Trap,
		ETraitEnum::Light
	};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomTypeEnum RoomType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTTNode> Traits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTraitSpawnChance> DefaultTraitsSpawnChance;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeByRoomType(ERoomTypeEnum NewRoomType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeDefault();

	UFUNCTION(BlueprintCallable)
	float GetFitnessValue() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	URoomTraits* Crossover(URoomTraits* Other);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Mutate(float GrowChance, float CutChance, int32 MinIterationCount, int32 MaxIterationCount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Grow();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Cut();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Alter();

	UFUNCTION(BlueprintCallable)
	URoomTraits* GetCopy(UObject* Owner) const;
};
