

#pragma once

#include "CoreMinimal.h"
#include "PCGAlgorithm.generated.h"


class UPCGTree;
class ARoom;
class UPCGSettings;


UCLASS(BlueprintType, Blueprintable)
class HORRORGAME_API UPCGAlgorithm : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPCGSettings> SettingsClass;

	UPROPERTY(BlueprintReadWrite, Transient)
	UPCGSettings* Settings;

	UPROPERTY(BlueprintReadWrite, Transient)
	AActor* ActorOwner = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient)
	UPCGTree* MainTree = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPCGTree> TreeClass;

	virtual UWorld* GetWorld() const override;

	void Initialize(AActor* Owner);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Run();
};
