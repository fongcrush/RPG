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
struct FItemStaticBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = -1))
	EItemType Type;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = -1))
	EItemQuality Quality;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = -1))
	FText Name;


	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = 10))
	float Weight;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = 10))
	bool bIsStackable;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = 10, EditConditionHides, EditCondition = "bIsStackable"))
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = 10))
	float SellValue;


	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = 10))
	FText InteractionText;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = 10))
	FText UsageText;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = 10))
	FText Description;


	UPROPERTY(EditDefaultsOnly, meta = (DisplayPriority = 20, ShowOnlyInnerProperties))
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

USTRUCT()
struct FBagStaticData : public FItemStaticBase
{
	GENERATED_BODY()

	FBagStaticData(): WeightCapacity(100), Width(1), Height(1)
	{
		Type = EItemType::Mundane;
		Quality = EItemQuality::Normal;
		Weight = 0.0f;
		bIsStackable = false;
		MaxStackSize = 1;
		SellValue = 0.0f;
		InteractionText = FText::FromString(TEXT("줍기"));
		UsageText = FText::FromString(TEXT("가방을 엽니다."));
		Description = FText::FromString(TEXT("가방입니다."));
	}

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 0))
	float WeightCapacity;	

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 1))
	int32 Width;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 1))
	int32 Height;

	UPROPERTY(EditDefaultsOnly, meta=(DisplayPriority = 10))
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
};
