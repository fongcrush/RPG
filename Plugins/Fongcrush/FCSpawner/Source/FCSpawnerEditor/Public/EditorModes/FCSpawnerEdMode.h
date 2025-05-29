// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

class USpawnerSubSystem;
/**
 * 
 */
class FCSPAWNEREDITOR_API FFCSpawnerEdMode final : public FEdMode
{
public:
	static FEditorModeID ModeID;

private:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	virtual void Initialize() override;
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	
	TSharedPtr<SWidget> GenerateContextMenu() const;

public:
	/** 선택 여부 변경 및 윤곽선 처리 */
	void SelectSpawner(const TWeakObjectPtr<UObject>& InSpawner, bool bIsSelected = true) const;
	/** 모든 Spawner 선택 해제 */
	void DeSelectAll() const;
private:	
	
	virtual bool UsesToolkits() const override { return true; }
	virtual bool IsCompatibleWith(FEditorModeID OtherModeID) const override { return true; }

private:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	TObjectPtr<UMaterialInterface> ValidMaterial;
	TObjectPtr<UMaterialInterface> ValidMaterial2;
	TObjectPtr<UMaterialInterface> InvalidMaterial;
	TObjectPtr<UMaterialInterface> InvalidMaterial2;
	TObjectPtr<UMaterialInterface> SelectedMaterial;
	TObjectPtr<UMaterialInterface> SelectedMaterial2;

	FDelegateHandle OnSelectedHandle;
};
