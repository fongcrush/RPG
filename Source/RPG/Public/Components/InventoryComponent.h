// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventory;
class UInventoryWidget;
class UInventorySlotWidget;
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

	FItemAddResult() :
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::None),
	ResultMessage(FText::GetEmpty()),
	LeftQuantity(0)	{	}

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::None;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}

	static FItemAddResult AddedPartial(const int32& PartialAmountAdded, const int32& InLeftQuantity, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::Partial;
		AddedPartialResult.ResultMessage = ErrorText;
		AddedPartialResult.LeftQuantity = InLeftQuantity;
		return AddedPartialResult;
	}

	static FItemAddResult AddedAll(const int32& AmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::All;
		AddedAllResult.ResultMessage = ErrorText;
		return AddedAllResult;
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓

	/** 인덱스 0이 기본 인벤토리 */
	UPROPERTY(VisibleInstanceOnly, Category = "Inventory")
	TArray<TObjectPtr<UInventory>> Inventories;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float WeightCapacity;

public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UInventoryComponent();
	virtual void PostInitProperties() override;

	// Getter 〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetTotalWeight() const;

	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return WeightCapacity; }

	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetContents() const;

	// Setter 〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float InWeightCapacity) { WeightCapacity = InWeightCapacity; }

	FItemAddResult HandleAddItem(UItemBase* Item, const int32& Quantity);

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void BeginPlay() override;

	FItemAddResult HandleAddNoneStackable(UItemBase* Item) const;
	FItemAddResult HandleAddStackable(UItemBase* Item, const int32 Quantity);
};

using InventoryComponentPtr = TObjectPtr<UInventoryComponent>;
