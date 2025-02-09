// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ItemActor.h"

#include "Components/InventoryComponent.h"
#include "Framework/RPGCharacter.h"
#include "Items/ItemBase.h"
#include "UIs/RPGHUD.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
	
	Quantity = 1;
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup();
}

void AItemActor::InitializePickup()
{
	if (FItemDataBase* ItemData = ItemDataHandle.GetRow<FItemDataBase>(GetName()))
	{
		ItemReference = NewObject<UItemBase>(this);
		ItemReference->DataReference = ItemData;
		ItemReference->SetQuantity(Quantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}


void AItemActor::InitializeDrop(const TObjectPtr<UItemBase>& DropItem, int32 InQuantity)
{
	// TODO: 수정 필요
	ItemReference = DropItem;
	
	DropItem->Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->GetDataReference()->Weight = DropItem->GetItemSingleWeight();
	ItemReference->OwingInventory = nullptr;
	PickupMesh->SetStaticMesh(DropItem->GetDataReference()->AssetData.Mesh);

	UpdateInteractableData();
}

void AItemActor::UpdateInteractableData()
{
	InteractableData.InteractableType = EInteractableType::Pickup;
	InteractableData.Action = ItemReference->GetDataReference()->InteractionText;
	InteractableData.Name = ItemReference->GetDataReference()->Name;
	InteractableData.Quantity = ItemReference->Quantity;
}

void AItemActor::BeginFocus()
{
	
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void AItemActor::EndFocus()
{
	if (PickupMesh)
	{
		
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void AItemActor::Interact(TObjectPtr<ARPGCharacter> InteractionCharacter)
{
	if (InteractionCharacter)
	{
		TakePickup(InteractionCharacter);
	}
}

void AItemActor::TakePickup(const TObjectPtr<ARPGCharacter>& Taker)
{
	if (!IsPendingKillPending())
	{
		if (!ItemReference)
		{
			UE_LOG(LogTemp, Warning, TEXT("인벤토리 속 아이템 참조가 null 입니다"));
			return;
		}
		
		// TODO: 플레이어 인벤토리에 아이템 추가 및 조정
		if (const TObjectPtr<UInventoryComponent> PlayerInventory = Taker->GetInventory())
		{
			const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

			switch (AddResult.OperationResult)
			{
			case EItemAddResult::None:
				break;

			case EItemAddResult::Partial:
				UpdateInteractableData();
				Taker->UpdateInteractionWidget();
				break;

			case EItemAddResult::All:
				Destroy();
				break;
			}

			UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Inventory is null!"));
		}
	}
}

#if WITH_EDITOR
void AItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.GetPropertyName() : NAME_None;
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		if (const FItemDataBase* ItemData = ItemDataHandle.GetRow<FItemDataBase>(GetName()))
		{
			PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
		}
	}
}
#endif
