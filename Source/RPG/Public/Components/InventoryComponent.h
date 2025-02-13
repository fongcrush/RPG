// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;
class UItemSlotWidget;
class UItemStackBase;

UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	None UMETA(DisplayName = "None"),
	Player UMETA(DisplayName = "Player"),
	Storage UMETA(DisplayName = "Storage"),
	Shop UMETA(DisplayName = "Shop")
};

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
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	EInventoryType Type;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float WeightCapacity;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Weight;
	
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TMap<TObjectPtr<UItemStackBase>,TObjectPtr<UItemSlotWidget>> InventoryMap;
	
	TArray<int32> FilledSlots;
	TArray<int32> EmptySlots;

public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UInventoryComponent();

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemStackBase* Item);

	UFUNCTION(Category = "Inventory")
	void RemoveStack(UItemStackBase* ItemStack);
	
	UFUNCTION(Category = "Inventory")
	int32 RemoveStackQuantity(UItemStackBase* Item, const int32 RequestedQuantity);
	
	UFUNCTION(Category = "Inventory")
	void SplitStack(UItemStackBase* Item, const int32 Quantity);

	// Getter 〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetTotalWeight() const { return Weight; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return WeightCapacity; }
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemStackBase*> GetContents() const;
	
	// Setter 〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { WeightCapacity = NewWeightCapacity; }
	
	FORCEINLINE bool Contains(const TObjectPtr<UItemStackBase>& ItemStack) const { return InventoryMap.Contains(ItemStack); };

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void BeginPlay() override;

	void Register();
	void RegisterSlots();

	FItemAddResult HandleAddNoneStackable(const TObjectPtr<UItemStackBase>& Item);
	FItemAddResult HandleAddStackable(const TObjectPtr<UItemStackBase>& ItemStack, const int32 RequestedQuantity);
	void AddNewStack(const TObjectPtr<UItemStackBase>& ItemStack, int32 Quantity);
	void SetWeight(const float& InWeight);
	
	FORCEINLINE int32 GetItemQuantityCanAdd(const TObjectPtr<UItemStackBase>& Item, int32 Quantity) const;	
	FORCEINLINE UItemSlotWidget* FindSlot(const TObjectPtr<UItemStackBase>& ItemStack) const { return *InventoryMap.Find(ItemStack); }
	FORCEINLINE UItemStackBase* FindNoneFullStack(const UItemStackBase* ItemStack) const;

};