// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItemActor;
class UInventory;
class UInventoryWidget;
class UItemSlotWidget;
class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	None UMETA(DisplayName = "None"),
	Partial UMETA(DisplayName = "Partial"),
	All UMETA(DisplayName = "All")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 LeftQuantity;
	
	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		return FItemAddResult(0, EItemAddResult::None, ErrorText, 0);
	}
	static FItemAddResult AddedPartial(const int32& PartialAmountAdded, const int32& InLeftQuantity, const FText& ErrorText)
	{
		return FItemAddResult(PartialAmountAdded, EItemAddResult::Partial, ErrorText, InLeftQuantity);
	}
	static FItemAddResult AddedAll(const int32& AmountAdded, const FText& ErrorText)
	{
		return FItemAddResult(AmountAdded, EItemAddResult::All, ErrorText, 0);
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓

	UInventoryComponent();
	virtual void PostInitProperties() override;
	
	/** 인벤토리에 아이템 추가 */
	FItemAddResult HandleAddItem(UItemBase* const& Item, const int32& Quantity);

	// Getter 〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	FORCEINLINE float GetMaxWeight() const { return MaxWeight; }
	FORCEINLINE float GetTotalWeight() const;

	// Setter 〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float InWeightCapacity) { MaxWeight = InWeightCapacity; }

protected:
	virtual void BeginPlay() override;
	/** 쌓을 수 있는 아이템 처리 */
	FItemAddResult HandleAddStackable(UItemBase* const& Item, const int32& Quantity);
	/** 이미 존재하는 스택에 추가 */
	int32 HandleAddToExistingStack(UItemBase* const& Item, const int32& Quantity);
	/** 빈 슬롯에 추가 */
	int32 HandleAddToEmptySlot(UItemBase* const& Item, const int32& Quantity);
	/** 쌓을 수 없는 아이템 처리 */
	FItemAddResult HandleAddNoneStackable(UItemBase* const& Item) const;

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UInventory>> Inventories;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TSubclassOf<UInventory>> DefaultInventoryClasses;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float MaxWeight;
};

using InventoryComponentPtr = TObjectPtr<UInventoryComponent>;
