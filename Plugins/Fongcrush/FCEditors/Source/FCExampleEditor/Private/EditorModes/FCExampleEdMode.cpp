﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorModes/FCExampleEdMode.h"

#include "Objects/FCExampleTargetPoint.h"
#include "Toolkits/FCExampleEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "Selection.h"

// 에디터 모드 ID 설정
const FEditorModeID FFCExampleEdMode::ModeID(TEXT("퐁크러시"));

// 히트 프록시 구현
IMPLEMENT_HIT_PROXY(HFCPointProxy, HHitProxy);

void FFCExampleEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShared<FFCExampleEdModeToolkit>();
		Toolkit->Init(Owner->GetToolkitHost());
	}

	// 초기화
	CurrentSelectedPointActor = nullptr;
	CurrentSelectedIndex = -1;
	
	// 명령 맵핑
	BindCommands();
}

void FFCExampleEdMode::Exit()
{
	FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
	Toolkit.Reset();

	FEdMode::Exit();
}

AFCExampleTargetPoint* GetSelectedTargetPointActor()
{
	// 월드에서 하나의 포인트 엑터만 선택됐을 경우에만 반환 함
	TArray<UObject*> SelectedObjects;
	GEditor->GetSelectedActors()->GetSelectedObjects(SelectedObjects);
	if (SelectedObjects.Num() == 1)
	{
		return Cast<AFCExampleTargetPoint>(SelectedObjects[0]);
	}
	return nullptr;
};

void FFCExampleEdMode::AddPoint()
{
	if (AFCExampleTargetPoint* SelectedPointActor = GetSelectedTargetPointActor())
	{
		// 트랜잭션(do / undo) 객체
		const FScopedTransaction Transaction(FText::FromString("Add Point"));
		// 트랜젝션에 현재 상태 저장 
		SelectedPointActor->Modify();

		// 현재 뷰포트의 카메라 위치를 기준으로 새 점 위치 설정. 
		const FEditorViewportClient* EditorViewportClient = StaticCast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
		const FVector NewPosition = EditorViewportClient->GetViewLocation() + EditorViewportClient->GetViewRotation().Vector() * 50.f;

		// 새로운 점 추가
		SelectedPointActor->Points.Add(NewPosition);

		// 새로운 점 선택
		SelectPoint(SelectedPointActor, SelectedPointActor->Points.Num() - 1);
	}
}

void FFCExampleEdMode::RemovePoint()
{
	if (HasValidSelection())
	{
		// 트랜젝션에 현재 상태 저장 
		CurrentSelectedPointActor->Modify();
		// 트랜잭션(do / undo) 객체
		const FScopedTransaction Transaction(FText::FromString("Remove Point"));

		// 선택된 점 삭제
		CurrentSelectedPointActor->Points.RemoveAt(CurrentSelectedIndex);

		// 점 선택 해제
		SelectPoint(nullptr, -1);
	}
}

bool FFCExampleEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	// HFCPointProxy를 감지했을 경우
	if (HitProxy && HitProxy->IsA(HFCPointProxy::StaticGetType()))
	{
		const HFCPointProxy* FCPointProxy = StaticCast<HFCPointProxy*>(HitProxy);
		AFCExampleTargetPoint* PointActor = Cast<AFCExampleTargetPoint>(FCPointProxy->RefObject);

		const int32& PointIndex = FCPointProxy->Index;
		if (PointActor && PointIndex >= 0 && PointIndex < PointActor->Points.Num())
		{
			// 점 선택
			SelectPoint(PointActor, PointIndex);
		}

		// 점 우클릭 시 컨텍스트 메뉴 생성
		if (Click.GetKey() == EKeys::RightMouseButton)
		{
			TSharedPtr<SWidget> MenuWidget = GenerateContextMenu(InViewportClient);
			if (MenuWidget.IsValid())
			{
				FSlateApplication::Get().PushMenu(
					Owner->GetToolkitHost()->GetParentWidget(),
					FWidgetPath(),
					MenuWidget.ToSharedRef(),
					FSlateApplication::Get().GetCursorPos(),
					FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));
			}
		}
		return true;
	}
	return false;
}


void FFCExampleEdMode::SelectPoint(AFCExampleTargetPoint* const& InTargetPoint, const int32& InIndex)
{
	CurrentSelectedPointActor = InTargetPoint;
	CurrentSelectedIndex = InIndex;

	// select this actor only
	if (CurrentSelectedPointActor.IsValid())
	{
		// 선택 해제 후 요청받은 엑터 선택
		GEditor->SelectNone(true, true);
		GEditor->SelectActor(CurrentSelectedPointActor.Get(), true, true);
	}
}

bool FFCExampleEdMode::HasValidSelection() const
{
	return CurrentSelectedPointActor.IsValid() && CurrentSelectedIndex >= 0 && CurrentSelectedIndex < CurrentSelectedPointActor->Points.Num();
}

void FFCExampleEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	constexpr FColor NormalColor(200, 200, 200);
	constexpr FColor SelectedColor(255, 128, 0);

	// 월드의 모든 FCTargetPoint 순회
	for (TActorIterator<AFCExampleTargetPoint> It(GetWorld()); It; ++It)
	{
		AFCExampleTargetPoint* PointActor = (*It);
		FVector ActorLocation = PointActor->GetActorLocation();

		for (int Index = 0; Index < PointActor->Points.Num(); ++Index)
		{
			// 색 구분
			const bool bSelected = (PointActor == CurrentSelectedPointActor && Index == CurrentSelectedIndex);
			const FColor& DrawColor = bSelected ? SelectedColor : NormalColor;

			// 히트 프록시 생성 및 설정
			HFCPointProxy* PointProxy = new HFCPointProxy(PointActor, Index);
			PDI->SetHitProxy(PointProxy);
			// 점 그리기
			PDI->DrawPoint(PointActor->Points[Index], DrawColor, 15.f, SDPG_Foreground);
			// 점와 엑터 위치를 연결하는 선 그리기
			PDI->DrawLine(PointActor->Points[Index], ActorLocation, DrawColor, SDPG_Foreground);
			// 히트 프록시 해제
			PDI->SetHitProxy(nullptr);
		}
	}

	FEdMode::Render(View, Viewport, PDI);
}

bool FFCExampleEdMode::InputDelta(
	FEditorViewportClient* InViewportClient,
	FViewport* InViewport,
	FVector& InDrag,
	FRotator& InRot,
	FVector& InScale)
{
	// FScopedTransaction 사용 X, 이미 내부적으로 트랜젝션이 처리 됨 

	// 현재 뷰포트 축이 None이면 아무 작업 패스
	if (InViewportClient->GetCurrentWidgetAxis() == EAxisList::None)
	{
		return false;
	}

	// Point 선택이 있는 경우
	if (HasValidSelection())
	{
		if (!InDrag.IsZero())
		{
			// 트랜젝션에 현재 상태 저장
			CurrentSelectedPointActor->Modify();
			// 현재 선택된 점의 위치를 드래그 벡터만큼 이동
			CurrentSelectedPointActor->Points[CurrentSelectedIndex] += InDrag;
		}
		return true;
	}
	return false;
}

FVector FFCExampleEdMode::GetWidgetLocation() const
{
	if (HasValidSelection())
	{
		return CurrentSelectedPointActor->Points[CurrentSelectedIndex];
	}
	return FEdMode::GetWidgetLocation();
}

/**
 * 명령 집합 클래스
 */
class FFCEditorCommands : public TCommands<FFCEditorCommands>
{
public:
	FFCEditorCommands() :
		TCommands<FFCEditorCommands>
		(
			"FCExampleEditor", // 조회명
			FText::FromString(TEXT("퐁크러시 테스트 에디터")), // 표시명
			NAME_None,
			FAppStyle::GetAppStyleSetName()
		){}

#define LOCTEXT_NAMESPACE "FCEditorCommands"

	virtual void RegisterCommands() override
	{
		UI_COMMAND(DeletePoint,
			"Delete Point",
			"Delete the currently selected point.",
			EUserInterfaceActionType::Button,
			FInputGesture(EKeys::Delete));
	}
#undef LOCTEXT_NAMESPACE

public:
	/** 점 삭제 명령 정보 */
	TSharedPtr<FUICommandInfo> DeletePoint;
};

FFCExampleEdMode::FFCExampleEdMode()
{
	// 명령 집합 클래스 등록
	FFCEditorCommands::Register();

	// 명령 컨트롤러 초기화
	UICommandList = MakeShared<FUICommandList>();
}

FFCExampleEdMode::~FFCExampleEdMode()
{
	FFCEditorCommands::Unregister();
}

void FFCExampleEdMode::BindCommands()
{
	const FFCEditorCommands& Commands = FFCEditorCommands::Get();
	
	// 삭제 명령에 로직 등록
	UICommandList->MapAction(
		Commands.DeletePoint,
		FExecuteAction::CreateSP(this, &FFCExampleEdMode::RemovePoint),
		FCanExecuteAction::CreateSP(this, &FFCExampleEdMode::HasValidSelection));
}

bool FFCExampleEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (Event == IE_Pressed)
	{
		// 명령 컨트롤러 처리
		return UICommandList->ProcessCommandBindings(Key, FSlateApplication::Get().GetModifierKeys(), false);
	}
	return false;
}

TSharedPtr<SWidget> FFCExampleEdMode::GenerateContextMenu(FEditorViewportClient* InViewportClient) const
{
	FMenuBuilder MenuBuilder(true, nullptr);

	// 명령 집합 클래스 등록
	MenuBuilder.PushCommandList(UICommandList.ToSharedRef());
	MenuBuilder.BeginSection("FC Section"); // FC Section
	// 메뉴 항목 추가
	if (HasValidSelection())
	{
		// 현재 선택된 점의 인덱스 화면에 표시
		const TSharedRef<SWidget> LabelWidget(
			SNew(STextBlock).Text(FText::FromString(FString::FromInt(CurrentSelectedIndex)))
			.ColorAndOpacity(FLinearColor::Green));
		
		MenuBuilder.AddWidget(LabelWidget, FText::FromString(TEXT("Point Index: ")));
		// 구분선 추가
		MenuBuilder.AddMenuSeparator();
		// 점 삭제 명령 추가
		MenuBuilder.AddMenuEntry(FFCEditorCommands::Get().DeletePoint);
	}
	MenuBuilder.EndSection(); // ~ FC Section
	MenuBuilder.PopCommandList();

	TSharedPtr<SWidget> MenuWidget = MenuBuilder.MakeWidget();
	return MenuWidget;
}
