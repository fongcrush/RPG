#pragma once
#include "ComponentVisualizer.h"


class FCSPAWNEREDITOR_API FFCSpawnerComponentVisualizer : public FComponentVisualizer
{
	friend class FFCSpawnerEdMode;
	
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	/** 초기화 */
	virtual void OnRegister() override;
	void RegisterSelectionChangedEvent();
	void UnRegisterSelectionChangedEvent();
	
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;

	/**
	 * FComponentVisualizerManager::HandleClick() 참고
	 * ((Control || Alt || Shift) && Click == true) -> (HandleModifiedClick == false) -> (VisProxyHandleClick == true && RightClick) -> GenerateContextMenu
	 */

	/** (Control || Alt || Shift) + Click */
	virtual bool HandleModifiedClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	/** HandleModifiedClick == false일 경우 호출 */ 
	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	
	/** VisProxyHandleClick == true && RightClick일 경우 호출 */
	virtual TSharedPtr<SWidget> GenerateContextMenu() const override;

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
	TArray<UObject*> SelectedSpawners;
};
