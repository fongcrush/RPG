// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

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
	
	virtual void RegisterSelectionEvent();
	virtual void UnRegisterBindSelectionEvents();
	virtual void OnSelected(UObject* Object);
	
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	
	TSharedPtr<SWidget> GenerateContextMenu() const;

	void SelectSpawner(const TWeakObjectPtr<UObject>& Spawner, bool bIsSelected = true) const;
	void ResetSelection();
	
	/** 유효하지 않은 SelectedSpawners 요소 제거 */
	void RemoveInvalidSelectedRefs() const;
	
	virtual bool UsesToolkits() const override { return true; }
	virtual bool IsCompatibleWith(FEditorModeID OtherModeID) const override { return true; }

public:
	FORCEINLINE TSet<TWeakObjectPtr<UObject>> GetSelectedSpawners() const { return SelectedSpawners; }

private:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	TObjectPtr<UMaterial> ValidMaterial;
	TObjectPtr<UMaterial> ValidMaterial2;
	TObjectPtr<UMaterial> InvalidMaterial;
	TObjectPtr<UMaterial> InvalidMaterial2;
	TObjectPtr<UMaterial> SelectedMaterial;
	TObjectPtr<UMaterial> SelectedMaterial2;

	mutable TSet<TWeakObjectPtr<UObject>> SelectedSpawners;

	FDelegateHandle OnSelectedHandle;
};
