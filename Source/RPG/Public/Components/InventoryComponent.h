// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryChanged);

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
	
	FItemAddResult() :
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::None),
	ResultMessage(FText::GetEmpty())
	{}

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::None;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}
	
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::Partial;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	}
	
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::All;
		AddedAllResult.ResultMessage = ErrorText;
		return AddedAllResult;		
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInventoryChanged OnInventoryChanged;
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UInventoryComponent();

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* Item);

	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* Item) const;
	
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* Item) const;
	
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* Item) const;

	UFUNCTION(Category = "Inventory")
	void RemoveSingleStack(UItemBase* Item);
	
	UFUNCTION(Category = "Inventory")
	int32 RemoveMultipleStack(UItemBase* Item, const int32 Quantity);
	
	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* Item, const int32 Quantity);

	// Getter 〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetTotalWeight() const { return TotalWeight; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return WeightCapacity; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents;}
	
	// Setter 〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { WeightCapacity = NewWeightCapacity; }
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float TotalWeight;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float WeightCapacity;
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void BeginPlay() override;

	int32 CalculateQuantityCanAdd(const TObjectPtr<UItemBase>& Item, const int32 Quantity) const;
	int32 CalculateQuantityForFull(const TObjectPtr<UItemBase>& Item, const int32 Quantity) const;
	FItemAddResult HandleNoneStackableItems(const TObjectPtr<UItemBase>& Item, const int32 Quantity);
	int32 HandleStackableItems(const TObjectPtr<UItemBase>& Item, const int32 Quantity);
	void AddNewItem(const TObjectPtr<UItemBase>& Item, int32 Quantity);
};
