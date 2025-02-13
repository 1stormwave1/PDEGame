
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/BodyItem.h"
#include "Inventory/MicrochipItem.h"
#include "InventoryComponent.generated.h"

class AElectricItemActor;
class UItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveSlotIndexChangedSignature, int32, NewCurrentActiveSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotItemChangedSignature, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClearedSignature, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPutInSlotSignature, int32, SlotIndex, UItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUpSignature, TSubclassOf<UItem>, ItemClass);

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Transient)
	TObjectPtr<UItem> CurrentItem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItem> CurrentItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InputAction = nullptr;
};

USTRUCT(BlueprintType)
struct FInventoryBodySlot : public FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBodyType BodyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	TObjectPtr<AItemActor> ItemActor = nullptr;
};

USTRUCT(BlueprintType)
struct FInventoryMicrochipSlot : public FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMicrochipType MicrochipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	TObjectPtr<AItemActor> ItemActor = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ActiveSlotIndex = 0;

	UPROPERTY(Transient, BlueprintReadWrite)
	TObjectPtr<class AItemActor> CurrentItemActor = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<class AHorrorGameController> OwnerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAdditionalSlotsActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropRandomCircleRadius = 10.f;
	
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slots")
	TArray<FInventorySlot> MainSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slots")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slots")
	TArray<FInventorySlot> AdditionalInventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slots")
	TArray<FInventoryBodySlot> BodySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slots")
	TArray<FInventoryMicrochipSlot> MicrochipSlots;
	
	UPROPERTY(BlueprintAssignable)
	FOnActiveSlotIndexChangedSignature OnActiveSlotIndexChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSlotItemChangedSignature OnActiveItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSlotItemChangedSignature OnBodySlotItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSlotItemChangedSignature OnMicrochipSlotItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSlotItemChangedSignature OnInventorySlotItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSlotItemChangedSignature OnMainInventorySlotItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSlotItemChangedSignature OnAdditionalSlotItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSlotClearedSignature OnMainSlotCleared;

	UPROPERTY(BlueprintAssignable)
	FOnSlotClearedSignature OnInventorySlotCleared;

	UPROPERTY(BlueprintAssignable)
	FOnSlotClearedSignature OnAdditionalSlotCleared;

	UPROPERTY(BlueprintAssignable)
	FOnSlotClearedSignature OnBodySlotCleared;

	UPROPERTY(BlueprintAssignable)
	FOnSlotClearedSignature OnMicrochipSlotCleared;

	UPROPERTY(BlueprintAssignable)
	FOnItemPutInSlotSignature OnItemPutInMainSlot;

	UPROPERTY(BlueprintAssignable)
	FOnItemPutInSlotSignature OnItemPutInInventorySlot;

	UPROPERTY(BlueprintAssignable)
	FOnItemPutInSlotSignature OnItemPutInAdditionalSlot;

	UPROPERTY(BlueprintAssignable)
	FOnItemPutInSlotSignature OnItemPutInBodySlot;

	UPROPERTY(BlueprintAssignable)
	FOnItemPutInSlotSignature OnItemPutInMicrochipSlot;

	UPROPERTY(BlueprintAssignable)
	FOnItemPickedUpSignature OnItemPickedUp;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentActiveSlotIndex() const { return ActiveSlotIndex; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AItemActor* GetCurrentItemActor() const { return CurrentItemActor; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCurrentItemActor(const AItemActor* Actor) { CurrentItemActor = nullptr; }
	
	void ClearInventorySlot(int32 Index, TArray<FInventorySlot>& InventoryArray);

	UFUNCTION(BlueprintCallable)
	void ClearNonQASlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	void ClearQASlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	void ClearAdditionalSlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	void ClearBodySlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	void ClearMicrochipSlot(int32 Index);

	void PutItemInInventory(UItem* Item, int32 Index, TArray<FInventorySlot>& InventoryArray);

	UFUNCTION(BlueprintCallable)
	void PutItemInNonQASlots(UItem* Item, int32 Index);

	UFUNCTION(BlueprintCallable)
	void PutItemInQASlots(UItem* Item, int32 Index);

	UFUNCTION(BlueprintCallable)
	void PutItemInAdditionalSlots(UItem* Item, int32 Index);

	UFUNCTION(BlueprintCallable)
	void PutItemInBodySlots(UItem* Item, int32 Index);

	UFUNCTION(BlueprintCallable)
	bool PutItemInBodySlotByType(UItem* Item, EBodyType Type);

	UFUNCTION(BlueprintCallable)
	bool PutItemInBodySlotByTypeWithActor(AItemActor* Actor, UItem* Item, EBodyType Type);

	UFUNCTION(BlueprintCallable)
	void PutItemInMicrochipSlots(UItem* Item, int32 Index);

	UFUNCTION(BlueprintCallable)
	void SetActiveSlotIndex(const int32 NewActiveSlotIndex = 0);

	UFUNCTION(BlueprintCallable)
	void SpawnActiveSlotItem(USceneComponent* AttachComponent);

	UFUNCTION(BlueprintCallable)
	bool PickUpItem(UItem* Item);

	UFUNCTION(BlueprintCallable)
	void DestroyActiveSlotItem();

	UFUNCTION(BlueprintCallable)
	void GetBodySlotByType(EBodyType Type, FInventoryBodySlot& Slot);

	UFUNCTION(BlueprintCallable)
	void SetOwnerController(AHorrorGameController* NewOwnerController);

	UFUNCTION(BlueprintCallable)
	UItem* GetCurrentActiveItem() const { return MainSlots[ActiveSlotIndex].CurrentItem; }

	UFUNCTION(BlueprintCallable)
	void ActivateBodyActor(AItemActor* Actor, EBodyType BodyType);

	UFUNCTION(BlueprintPure)
	AItemActor* GetItemActorByBodyType(EBodyType BodyType);

	UFUNCTION(BlueprintCallable)
	void SetItemActorForBodyType(AItemActor* Actor, EBodyType BodyType);
	
	UFUNCTION(BlueprintPure)
	AItemActor* GetItemActorByMicrochipType(EMicrochipType MicrochipType);

	UFUNCTION(BlueprintPure)
	void GetAllFillUpItems(UItem* PowerItem, TArray<UFillUpItem*>& FillUpItems);

	UFUNCTION(BlueprintPure)
	void GetAllLimitedPowerItems(UFillUpItem* FillUpItem, TArray<UItem*>& PowerItems);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromInventory(UItem* Item);

	UFUNCTION(BlueprintPure)
	void GetAllActiveElectricItemActors(TArray<AElectricItemActor*>& ElectricItemActors);

	UFUNCTION(BlueprintPure)
	bool DoesContainItem(UItem* Item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DropItem(FInventorySlot& Slot, FVector Location);

	UFUNCTION(BlueprintCallable)
	void DropItemFromMainSlots(int32 Index, FVector Location);

	UFUNCTION(BlueprintCallable)
	void DropItemFromInventorySlots(int32 Index, FVector Location);

	UFUNCTION(BlueprintCallable)
	void DropItemFromAdditionalSlots(int32 Index, FVector Location);

	UFUNCTION(BlueprintCallable)
	void DropItemFromBodySlots(int32 Index, FVector Location);

	UFUNCTION(BlueprintCallable)
	void DropItemFromMicrochipSlots(int32 Index, FVector Location);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FVector GetDropLocation();
};
