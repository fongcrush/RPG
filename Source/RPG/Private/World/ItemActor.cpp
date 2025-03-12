// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ItemActor.h"

#include "Components/InventoryComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Items/ItemBase.h"
#include "RPG/RPG.h"
#include "UIs/RPGHUD.h"
#include "UIs/Interaction/InteractionWidget.h"


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
	if (!ItemClass)
	{
		LOG_WARNING("%s: ItemClass is nullptr", *GetName());
		return;
	}
	HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	Item = ItemClass->GetDefaultObject<UItemBase>();
	PickupMesh->SetStaticMesh(Item->GetStaticData()->AssetData.Mesh);
	
	LoadInteractableData();
}

void AItemActor::InitializeDrop(const TObjectPtr<UItemBase>& ItemDropped, int32 InQuantity)
{
	if (!ItemDropped)
	{
		LOG_ERROR("%s: DropItem is nullptr", *GetName());
		return;
	}
	HUD = Cast<ARPGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	Item = ItemDropped;
	Quantity = InQuantity;
	PickupMesh->SetStaticMesh(ItemDropped->GetStaticData()->AssetData.Mesh);
	
	LoadInteractableData();
}

void AItemActor::LoadInteractableData()
{
	if (const FItemStaticBase* ItemStaticData = Item->GetStaticData())
	{
		InteractableData.Name = ItemStaticData->Name;
		InteractableData.InteractableType = EInteractableType::Pickup;
		InteractableData.InteractionText = ItemStaticData->InteractionText;
		InteractableData.Quantity = Quantity;
	}
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

void AItemActor::Interact(APawn* const& Interactor)
{
	if (APlayerCharacter* RPGCharacter = Cast<APlayerCharacter>(Interactor))
	{
		TakeItem(RPGCharacter);
	}
}

void AItemActor::EndInteract()
{
}

void AItemActor::TakeItem(APlayerCharacter* const& Taker)
{
	if (IsPendingKillPending())
	{
		LOG_CALLINFO("이미 사라진 아이템 입니다.");
		return;
	}
	if (!Item)
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
	LOG_CALLINFO("%s", *AddResult.ResultMessage.ToString());
	switch (AddResult.OperationResult) {
	case EItemAddResult::None:
		break;
	case EItemAddResult::Partial:
		InteractableData.Quantity = AddResult.LeftQuantity;
		HUD->InteractionWidget->UpdateWidget(InteractableData);
		break;
	case EItemAddResult::All:
		Destroy();		
		break;
	}
}

#if WITH_EDITOR
void AItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.GetPropertyName() : NAME_None;
	if (ItemClass && ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AItemActor, ItemClass))
	{
		UItemBase* ItemCDO = ItemClass.Get()->GetDefaultObject<UItemBase>();
		if (FItemStaticBase* ItemData = ItemCDO->GetStaticDataHandle().GetRow<FItemStaticBase>(GetName()))
		{
			LOG_CALLINFO("ItemActor: %s PostEditChangeProperty", *GetName());
			PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

			// DataTable에서 RowName이 변경되어도 Handle이 가리키는 RowName은 바뀌지가 않아서 수동 변경 해줘야 함
			// ItemData->OnRowNameChanged.AddLambda([&](FName RenamedRowName){ ItemDataHandle.RowName = RenamedRowName; });
		}
	}
}
#endif
