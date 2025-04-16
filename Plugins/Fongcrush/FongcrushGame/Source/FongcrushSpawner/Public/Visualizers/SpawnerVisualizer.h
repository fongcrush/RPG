#pragma once
#include "ComponentVisualizer.h"
#include "Components/SpawnerComponent.h"


class FSpawnerComponentVisualizer : public FComponentVisualizer
{
public:
	virtual void OnRegister() override;

	virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
	
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;

	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;

	virtual UActorComponent* GetEditedComponent() const override { return SelectedComponent.IsValid() ? SelectedComponent.Get() : nullptr; }

	virtual void EndEditing() override;
	
private:
	TWeakObjectPtr<USpawnerComponent> SelectedComponent;
	TObjectPtr<UMaterial> BottomMaterial;
	TObjectPtr<UMaterial> ValidMaterial;
	TObjectPtr<UMaterial> InValidMaterial;
};
