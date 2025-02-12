// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/MovingUserWidget.h"


void UMovingUserWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UMovingUserWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	SetPositionInViewport(InDragDropEvent.GetScreenSpacePosition() * 2 - InDragDropEvent.GetLastScreenSpacePosition());
	
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}