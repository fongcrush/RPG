﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ItemActor.h"

#include "Components/InventoryComponent.h"
#include "Framework/RPGCharacter.h"
#include "Items/ItemStackBase.h"
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
	if (FItemStaticBase* ItemData = ItemDataHandle.GetRow<FItemStaticBase>(GetName()))
	{
		ItemReference = NewObject<UItemStackBase>(this);
		ItemReference->StaticData = ItemData;
		ItemReference->SetQuantity(Quantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}


void AItemActor::InitializeDrop(const TObjectPtr<UItemStackBase>& DropItem, int32 InQuantity)
{
	// TODO: 수정 필요
	ItemReference = DropItem;
	
	DropItem->Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->GetStaticData()->Weight = DropItem->GetSingleWeight();
	PickupMesh->SetStaticMesh(DropItem->GetStaticData()->AssetData.Mesh);

	UpdateInteractableData();
}

void AItemActor::UpdateInteractableData()
{
	InteractableData.InteractableType = EInteractableType::Pickup;
	InteractableData.Action = ItemReference->GetStaticData()->InteractionText;
	InteractableData.Name = ItemReference->GetStaticData()->Name;
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
		if (ItemReference == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("인벤토리 속 아이템 참조가 null 입니다"));
			return;
		}
		
		// TODO: 플레이어 인벤토리에 아이템 추가 및 조정
		if (const TObjectPtr<UInventoryComponent> Inventory = Taker->GetInventory())
		{
			const FItemAddResult AddResult = Inventory->HandleAddItem(ItemReference);

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
		if (FItemStaticBase* ItemData = ItemDataHandle.GetRow<FItemStaticBase>(GetName()))
		{
			PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

			// DataTable에서 RowName이 변경되어도 Handle이 가리키는 RowName은 바뀌지가 않아서 수동 변경 해줘야 함
			// ItemData->OnRowNameChanged.AddLambda([&](FName RenamedRowName){ ItemDataHandle.RowName = RenamedRowName; });
		}
	}
}
#endif
