// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ItemActor.h"

#include "Components/InventoryComponent.h"
#include "Framework/RPGCharacter.h"
#include "Items/ItemBase.h"
#include "RPG/RPG.h"
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
	Initialize();
}

void AItemActor::Initialize()
{
	if (!Item)
	{
		LOG_ERROR("%s: ItemClass is nullptr", *GetName());
		return;
	}
	PickupMesh->SetStaticMesh(Item->GetStaticData()->AssetData.Mesh);
	UpdateInteractableData();
}

void AItemActor::InitializeDrop(const TObjectPtr<UItemBase>& DropItem, int32 InQuantity)
{
	// TODO: 호출쪽에서 DropItem를 CDO/Instance 복사본 중 어떤 걸로 넘겨줄지, 수량 전부/일부에 따라서 결정해야 함 
	if (!DropItem)
	{
		LOG_ERROR("%s: DropItem is nullptr", *GetName());
		return;
	}
	
	Item = DropItem;
	Quantity = InQuantity;

	PickupMesh->SetStaticMesh(DropItem->GetStaticData()->AssetData.Mesh);

	UpdateInteractableData();
}

void AItemActor::UpdateInteractableData()
{
	if (!Item)
	{
		LOG_ERROR("%s: ItemClass is nullptr", *GetName());
		return;
	}
	if (FItemStaticBase* StaticData = Item->GetStaticData())
	{
		InteractableData.InteractableType = EInteractableType::Pickup;
		InteractableData.Action = StaticData->InteractionText;
		InteractableData.Name = StaticData->Name;
	}
}

void AItemActor::TakePickup(const TObjectPtr<ARPGCharacter>& Taker)
{
	if (IsPendingKillPending())
	{
		LOG_CALLINFO("이미 사라진 아이템 입니다.");
		return;
	}
	if (Item == nullptr)
	{
		LOG_CALLINFO("유효한 아이템이 아닙니다.");
		return;
	}
	if (!Taker->GetInventoryComponent())
	{
		LOG_CALLINFO("플레이어 인벤토리가 유효하지 않습니다.");
		return;
	}
	
	// 인벤토리에 아이템 추가 및 조정
	const FItemAddResult AddResult = Taker->GetInventoryComponent()->HandleAddItem(Item, Quantity);
	switch (AddResult.OperationResult)
	{
	case EItemAddResult::None:
		break;
	case EItemAddResult::Partial:
		UpdateInteractableData();
		Taker->UpdateInteractionWidget();
		Quantity = AddResult.LeftQuantity;
		break;
	case EItemAddResult::All:
		Destroy();
		break;
	}
	LOG_CALLINFO("%s", *AddResult.ResultMessage.ToString());
}

#if WITH_EDITOR
void AItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.GetPropertyName() : NAME_None;
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		if (FItemStaticBase* ItemData = Item->GetStaticData())
		{
			PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

			// DataTable에서 RowName이 변경되어도 Handle이 가리키는 RowName은 바뀌지가 않아서 수동 변경 해줘야 함
			// ItemData->OnRowNameChanged.AddLambda([&](FName RenamedRowName){ ItemDataHandle.RowName = RenamedRowName; });
		}
	}
}
#endif
