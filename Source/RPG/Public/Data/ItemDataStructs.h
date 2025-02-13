#pragma once

#include "CoreMinimal.h"
#include "DataTableEditorUtils.h"
#include "DataTableUtils.h"
#include "ItemDataStructs.generated.h"

UENUM()
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Amor UMETA(DisplayName = "Amor"),
	Shield UMETA(DisplayName = "Shield"),
	Spell UMETA(DisplayName = "Spell"),
	Mundane UMETA(DisplayName = "Mundane"),
	Consumable UMETA(DisplayName = "Consumable"),
	Quest UMETA(DisplayName = "Quest"),
};

UENUM()
enum class EItemQuality : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Rare UMETA(DisplayName = "Rare"),
	Unique UMETA(DisplayName = "Unique"),
	Legendary UMETA(DisplayName = "Legend"),
	Mythical UMETA(DisplayName = "Mythical")
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

#if WITH_EDITOR
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRowNameChanged, FName)
#endif

USTRUCT()
struct FItemDataBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	EItemType Type;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	EItemQuality Quality;


	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	FText Name;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	FText Description;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	FText InteractionText;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	FText UsageText;


	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	float Weight;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	bool bIsStackable;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	float SellValue;


	UPROPERTY(EditDefaultsOnly, Category = "Item Data", meta = (DisplayPriority = 10))
	FItemAssetData AssetData;

	
	FOnRowNameChanged OnRowNameChanged;

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	// virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	// {
	// 	FName NewRawName = *(UEnum::GetDisplayValueAsText(Type).ToString()
	// 		+ TEXT("_") + UEnum::GetDisplayValueAsText(Quality).ToString()
	// 		+ TEXT("_") + Name.ToString());
	// 	NewRawName.SetNumber(1);
	//
	// 	while (InDataTable->FindRowUnchecked(NewRawName))
	// 	{
	// 		NewRawName.SetNumber(NewRawName.GetNumber() + 1);
	// 	}
	//
	// 	if (InRowName != NewRawName)
	// 	{
	// 		FDataTableEditorUtils::RenameRow(const_cast<UDataTable*>(InDataTable), InRowName, NewRawName);
	// 		FDataTableEditorUtils::SelectRow(InDataTable, NewRawName);
	// 		if (OnRowNameChanged.IsBound())
	// 		{
	// 			OnRowNameChanged.Broadcast(NewRawName);
	// 		}
	// 	}
	// }
	//
	// virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutProblems) override
	// {
	// 	FName NewRawName = *(UEnum::GetDisplayValueAsText(Type).ToString()
	// 		+ TEXT("_") + UEnum::GetDisplayValueAsText(Quality).ToString()
	// 		+ TEXT("_") + Name.ToString());
	// 	NewRawName.SetNumber(1);
	//
	// 	while (InDataTable->FindRowUnchecked(NewRawName))
	// 	{
	// 		NewRawName.SetNumber(NewRawName.GetNumber());
	// 	}
	//
	// 	if (InRowName != NewRawName)
	// 	{
	// 		FDataTableEditorUtils::RenameRow(const_cast<UDataTable*>(InDataTable), InRowName, NewRawName);
	// 		FDataTableEditorUtils::SelectRow(InDataTable, NewRawName);
	// 		if (OnRowNameChanged.IsBound())
	// 		{
	// 			OnRowNameChanged.Broadcast(NewRawName);
	// 		}
	// 	}
	// }
};