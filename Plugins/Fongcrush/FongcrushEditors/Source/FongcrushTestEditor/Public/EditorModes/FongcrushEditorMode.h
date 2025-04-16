// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "HitProxies.h"

class AFongcrushTargetPoint;

/**
 * HHitProxy - 유저 인터페이스 히트 감지 클래스
 * UE 의 에디터 렌더링 및 선택 시스템에서 사용하는 클래스
 * 뷰포트 상에서 마우스가 어떤 오브젝트를 클릭했는지를 식별하는데 쓰임
 */
struct HFongcrushPointProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();

	HFongcrushPointProxy(UObject* const& InRefObject, const int32& InIndex)
	: HHitProxy(HPP_UI), RefObject(InRefObject), Index(InIndex) {}

	TObjectPtr<UObject> RefObject;
	int32 Index;
};

/**
 * 
 */
class FONGCRUSHTESTEDITOR_API FFongcrushEditorMode : public FEdMode
{
public:
	const static FEditorModeID ModeID; // typedef FName

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	FFongcrushEditorMode();
	~FFongcrushEditorMode();
	
	/** 에디터 모드 진입 시 호출 */
	virtual void Enter() override;
	/** 에디터 모드 종료 시 호출 */
	virtual void Exit() override;

	/** 포인트 추가 */
	void AddPoint();
	/** 포인트 제거 */
	void RemovePoint();
	/** 선택된 포인트가 있는지? */
	bool HasValidSelection() const;
	/** 포인트 선택 */
	void SelectPoint(AFongcrushTargetPoint* const& TargetPoint, const int32& InIndex);
	/** 모드 진입 후 클릭시 수행되는 재정의 함수  */
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click) override;
	/** Render 함수 재정의 */
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	
	/**
	 * 뷰포트의 입력 변화 처리
	 * 기즈모 사용할 경우(UsesTransformWidget == true) InDrag(FVector) 값 자동 보정되어 들어옴
	 * */
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;
	/**  */
	virtual bool ShowModeWidgets() const override { return true; }
	/**  */
	virtual bool ShouldDrawWidget() const override { return true; }
	/** 기즈모 사용 */
	virtual bool UsesTransformWidget() const override { return true; }
	/**  */
	virtual FVector GetWidgetLocation() const override;

	/** 명령 맵핑 */
	void BindCommands();
	/** 키 입력 처리. */
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	/** 컨텍스트 메뉴 생성 */
	TSharedPtr<SWidget> GenerateContextMenu(FEditorViewportClient* InViewportClient) const;

	/**
	 * 그 외 에디터 모드에서 사용되는 주요 함수들 몇 가지
	 * virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	 * virtual bool CapturedMouseMove(FEditorViewportClient* InViewportClient, FViewport* InViewport, int32 InMouseX, int32 InMouseY) override;
	 * virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	 * virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	 * virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click) override;
	 * virtual void PostUndo() override;
	 * virtual void ActorsDuplicatedNotify(TArray<AActor*>& PreDuplicateSelection, TArray<AActor*>& PostDuplicateSelection, bool bOffsetLocations) override;
	 * virtual void ActorMoveNotify() override;
	 * virtual void ActorSelectionChangeNotify() override;
	 * virtual void MapChangeNotify() override;
	 * virtual void SelectionChanged() override;
	 */

protected:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	
	/** 현재 선택된 포인트 엑터 */
	TWeakObjectPtr<AFongcrushTargetPoint> CurrentSelectedPointActor;
	/** 현재 선택된 포인트 인덱스. 포인트 삭제시 참조 용도 */
	int32 CurrentSelectedIndex = -1;

	/** */
	TSharedPtr<FUICommandList> UICommandList;
};
