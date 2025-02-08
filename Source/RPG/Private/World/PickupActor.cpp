// Fill out your copyright notice in the Description page of Project Settings.


#include "World/PickupActor.h"

#include "Components/InventoryComponent.h"
#include "Framework/RPGCharacter.h"
#include "Items/ItemBase.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup();
}

void APickupActor::InitializePickup()
{
	for (FPickupInfo PickupInfo : PickupInfos)
	{
		const FName& RowName = PickupInfo.ItemDataHandle.RowName;
		const FItemDataBase* ItemData = PickupInfo.ItemDataHandle.DataTable->FindRow<FItemDataBase>(RowName, RowName.ToString());

		PickupInfo.ItemReference = NewObject<UItemBase>(this);

		PickupInfo.Quantity <= 0 ? PickupInfo.ItemReference->SetQuantity(1) : PickupInfo.ItemReference->SetQuantity(PickupInfo.Quantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickupActor::InitializeDrop(const TObjectPtr<UItemBase>& DropItem, int32 InQuantity)
{
	PickupInfos.Add(FPickupInfo(DropItem->ItemDataHandle, InQuantity, DropItem));
	DropItem->Quantity <= 0 ? DropItem->SetQuantity(1) : DropItem->SetQuantity(InQuantity);
	DropItem->GetDataReference()->Weight = DropItem->GetItemSingleWeight();
	PickupMesh->SetStaticMesh(DropItem->GetDataReference()->AssetData.Mesh);

	UpdateInteractableData();
}

void APickupActor::UpdateInteractableData()
{
	for (FPickupInfo PickupInfo : PickupInfos)
	{
		if (const TObjectPtr<UItemBase>& ItemReference = PickupInfo.ItemReference)
		{
			PickupInfo.InstanceInteractableData.InteractableType = EInteractableType::Pickup;
			PickupInfo.InstanceInteractableData.Action = ItemReference->GetDataReference()->InteractionText;
			PickupInfo.InstanceInteractableData.Name = ItemReference->GetDataReference()->Name;
			PickupInfo.InstanceInteractableData.Quantity = ItemReference->Quantity;
		}
	}
}

void APickupActor::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickupActor::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

FInteractableData* APickupActor::GetInteractableData()
{
	if (PickupInfos.IsEmpty())
	{
		return nullptr;
	}
	return PickupInfos.Num() > 1 ? &DefaultInteractableData : &PickupInfos[0].InstanceInteractableData;
}

void APickupActor::Interact(TObjectPtr<ARPGCharacter> InteractionCharacter)
{
	if (InteractionCharacter)
	{
		TakePickup(InteractionCharacter);
	}
}

void APickupActor::TakePickup(const TObjectPtr<ARPGCharacter>& Taker)
{
	if (!IsPendingKillPending())
	{
		for (const FPickupInfo& PickupInfo : PickupInfos)
		{
			if (PickupInfo.ItemReference)
			{
				// TODO: 플레이어 인벤토리에 아이템 추가 및 조정
				if (const TObjectPtr<UInventoryComponent> PlayerInventory = Taker->GetInventory())
				{
					const FItemAddResult AddResult = PlayerInventory->HandleAddItem(PickupInfo.ItemReference);

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
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("인벤토리 속 아이템 참조가 null 입니다"));
			}
		}
	}
}

#if WITH_EDITOR
void APickupActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PickupInfos.IsEmpty())
	{
		const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.GetPropertyName() : NAME_None;
		if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
		{
			const FName& RowName = PickupInfos[0].ItemDataHandle.RowName;
			if (const FItemDataBase* ItemData = PickupInfos[0].ItemDataHandle.DataTable->FindRow<FItemDataBase>(RowName, RowName.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}
#endif
