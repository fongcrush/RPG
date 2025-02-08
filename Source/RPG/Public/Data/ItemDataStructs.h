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
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{		
		const FName NewRawName = *(UEnum::GetDisplayValueAsText(Type).ToString() + TEXT("_") + UEnum::GetDisplayValueAsText(Quality).ToString() + TEXT("_") + Name.ToString());
		FDataTableEditorUtils::RenameRow(const_cast<UDataTable*>(InDataTable), InRowName, DuplicateSuffix(NewRawName, InDataTable));
	}

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutProblems) override
	{
		const FName NewRawName = *(UEnum::GetDisplayValueAsText(Type).ToString() + TEXT("_") + UEnum::GetDisplayValueAsText(Quality).ToString() + TEXT("_") + Name.ToString());
		FDataTableEditorUtils::RenameRow(const_cast<UDataTable*>(InDataTable), InRowName, DuplicateSuffix(NewRawName, InDataTable));	
	}

private:
	static FName DuplicateSuffix(const FName& InRowName, const UDataTable* InDataTable)
	{
		FName NewRawName = InRowName;
		int32 Suffix = 0;
		while (InDataTable->FindRowUnchecked(NewRawName))
		{
			NewRawName = FName(*(InRowName.ToString() + FString::Printf(TEXT("_%d"), Suffix)));
			++Suffix;
		}
		return NewRawName;
	}
};