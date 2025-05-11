// Fill out your copyright notice in the Description page of Project Settings.
#include "EditorModes/FCSpawnerEdMode.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "ComponentVisualizer.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "FCSpawnerEditor.h"
#include "Selection.h"
#include "UnrealEdGlobals.h"
#include "Components/SpawnerComponent.h"
#include "Editor/UnrealEdEngine.h"
#include "EditorModes/FCSpawnerModeToolkit.h"
#include "Settings/FCSpawnerSettings.h"
#include "SubSystems/SpawnerSubSystem.h"
#include "Toolkits/ToolkitManager.h"
#include "Visualizers/FCSpawnerVisualizer.h"

FEditorModeID FFCSpawnerEdMode::ModeID(TEXT("Spawner"));

void FFCSpawnerEdMode::Initialize()
{
	ValidMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/FCSpawner/M_Debug_Valid.M_Debug_Valid"));
	check(ValidMaterial)
	ValidMaterial2 = LoadObject<UMaterialInterface>(nullptr, TEXT("/FCSpawner/M_Debug_Valid_2.M_Debug_Valid_2"));
	check(ValidMaterial2)
	InvalidMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/FCSpawner/M_Debug_Invalid.M_Debug_Invalid"));
	check(InvalidMaterial)
	InvalidMaterial2 = LoadObject<UMaterialInterface>(nullptr, TEXT("/FCSpawner/M_Debug_Invalid_2.M_Debug_Invalid_2"));
	check(InvalidMaterial2)
	SelectedMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/FCSpawner/M_Debug_Selected.M_Debug_Selected"));
	check(SelectedMaterial)
	SelectedMaterial2 = LoadObject<UMaterialInterface>(nullptr, TEXT("/FCSpawner/M_Debug_Selected_2.M_Debug_Selected_2"));
	check(SelectedMaterial2)

	SpawnerSubSystem = GEngine->GetEngineSubsystem<USpawnerSubSystem>();
	check(SpawnerSubSystem)
}

void FFCSpawnerEdMode::Enter()
{
	FEdMode::Enter();

	// SpawnerVisualizer의 SelectionChangedEvent 등록 해제
	TSharedPtr<FComponentVisualizer> ComponentVisualizer = GUnrealEd->FindComponentVisualizer(USpawnerComponent::StaticClass()->GetFName());
	if (ComponentVisualizer.IsValid())
	{
		if (FFCSpawnerComponentVisualizer* SpawnerVisualizer = static_cast<FFCSpawnerComponentVisualizer*>(ComponentVisualizer.Get()))
		{
			SpawnerVisualizer->UnRegisterSelectionChangedEvent();
		}
	}

	// Toolkit 생성
	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShared<FFCSpawnerModeToolkit>();
		Toolkit->Init(Owner->GetToolkitHost());

		TArray<UObject*> SelectedSpawners;
		SpawnerSubSystem->GetSelectedSpawners(SelectedSpawners);
		StaticCastSharedPtr<FFCSpawnerModeToolkit>(Toolkit)->GetSpawnerDetailView()->SetObjects(SelectedSpawners);
	}
}

void FFCSpawnerEdMode::Exit()
{
	// SpawnerVisualizer의 SelectionChangedEvent 다시 등록
	TSharedPtr<FComponentVisualizer> ComponentVisualizer = GUnrealEd->FindComponentVisualizer(USpawnerComponent::StaticClass()->GetFName());
	if (ComponentVisualizer.IsValid())
	{
		if (FFCSpawnerComponentVisualizer* SpawnerVisualizer = static_cast<FFCSpawnerComponentVisualizer*>(ComponentVisualizer.Get()))
		{
			SpawnerVisualizer->RegisterSelectionChangedEvent();
		}
	}

	// 툴킷 종료
	FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
	Toolkit.Reset();

	// 선택 강조 해제
	for (const auto& Spawner : SpawnerSubSystem->Spawners)
	{
		for (auto& PreviewComp : Spawner->PreviewComponents)
		{
			PreviewComp->SetRenderCustomDepth(false); // 윤곽선 해제
		}
	}

	FEdMode::Exit();
}

void FFCSpawnerEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);

	for (const auto& Spawner : SpawnerSubSystem->Spawners)
	{
		bool bIsSelected = Spawner->IsSelectedInEdMode();

		const FVector SpawnerLocation = Spawner->GetComponentLocation();
		const float Distance = FVector::Dist(View->ViewLocation, SpawnerLocation);
		const float& MaxDrawDistance = UFCSpawnerSettings::GetMaxDebugDrawDistance();

		// 설정의 최대 거리 값보다 가까워야 통과
		if (Distance > MaxDrawDistance)
		{
			return;
		}

		// 시각화 그리기 시작
		PDI->SetHitProxy(new HComponentVisProxy(Spawner.Get()));
		{
			// 부모 엑터와의 연결 선
			PDI->DrawLine(Spawner->GetOwner()->GetActorLocation(), Spawner->GetComponentLocation(), FColor::Orange, SDPG_Foreground, 1.f);

			const FColor VisColor = bIsSelected ? FColor::Yellow : Spawner->GetSpawnActorClass() ? FColor::Green : FColor::Red;

			// 위치 표시용 점
			PDI->DrawPoint(SpawnerLocation, VisColor, 10.f, SDPG_Foreground);

			// 생성할 엑터 클래스가 없을 경우
			if (Spawner->GetSpawnActorClass() == nullptr)
			{
				const FMatrix ConeScaleMat = FScaleMatrix(100.f);
				const FMatrix ConeRotMat = FRotationMatrix(Spawner->GetComponentRotation() + FRotator(90, 0, 0));
				const FMatrix ConePosMat = FTranslationMatrix(SpawnerLocation);
				const FMatrix ArrowToWorld = ConeScaleMat * ConeRotMat * ConePosMat;

				const UMaterialInterface* ConeMaterial = bIsSelected ? SelectedMaterial2 : InvalidMaterial2;

				// 원뿔 옆면 (DrawCone은 밑면 렌더링 X)
				DrawCone(PDI,
				         ArrowToWorld,
				         FMath::DegreesToRadians(20.f), FMath::DegreesToRadians(20.f),
				         32, false,
				         FLinearColor::White,
				         ConeMaterial->GetRenderProxy(),
				         SDPG_World);

				// 원뿔 밑면(위쪽 뚜껑)
				DrawDisc(PDI,
				         SpawnerLocation + FVector(0.f, 0.f, 93.f),
				         FVector::XAxisVector, FVector::YAxisVector,
				         FColor::White,
				         34.f, 32,
				         ConeMaterial->GetRenderProxy(),
				         SDPG_World);

				const UMaterialInterface* BottomMaterial = bIsSelected ? SelectedMaterial : InvalidMaterial;

				// 바닥 구분용 원
				DrawDisc(PDI,
				         SpawnerLocation,
				         FVector::XAxisVector, FVector::YAxisVector,
				         VisColor,
				         25.f, 32,
				         BottomMaterial->GetRenderProxy(),
				         SDPG_World);
			}
		}
		// 시각화 그리기 종료
		PDI->SetHitProxy(nullptr);

		// 선택 강조 해제
		for (const auto& PreviewComp : Spawner->PreviewComponents)
		{
			PreviewComp->SetRenderCustomDepth(bIsSelected); // 선택 강조 해제
		}
	}
}

void FFCSpawnerEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);

	for (const auto& Spawner : SpawnerSubSystem->Spawners)
	{
		const FVector SpawnerLocation = Spawner->GetComponentLocation();
		const float Distance = FVector::Dist(View->ViewLocation, SpawnerLocation);
		const float& MaxDrawDistance = UFCSpawnerSettings::GetMaxDebugDrawDistance();

		// 설정의 최대 거리 값보다 가까워야 통과
		if (Distance > MaxDrawDistance) continue;

		// 시야에 들어올 경우에만 통과
		if (!View->ViewFrustum.IntersectPoint(SpawnerLocation)) continue;

		FVector2D ScreenPos;
		if (View->WorldToPixel(SpawnerLocation + FVector(0, 0, 50.f), ScreenPos))
		{
			ScreenPos /= Canvas->GetDPIScale();

			// 디버깅 문자열 초기화
			FString DebugString;
			if (const UClass* SpawnerClass = Spawner->GetSpawnActorClass())
			{
				DebugString += FString::Printf(TEXT("Spawn : %s\n"), *Spawner->GetSpawnActorClass()->GetName());
				DebugString += FString::Printf(TEXT("Spawn Delay: %.2fs"), Spawner->GetSpawnTime());
			}

			// 텍스트 아이템 초기화
			FCanvasTextItem TextItem(
				ScreenPos,
				DebugString.IsEmpty() ? FText::FromString("None") : FText::FromString(DebugString),
				GEngine->GetSmallFont(),
				FLinearColor::White);

			TextItem.Scale = FVector2D(FMath::GetMappedRangeValueClamped(
				FVector2D(200.f, MaxDrawDistance), // 거리 범위
				FVector2D(1.0f, 0.5f), // 스케일 범위 (가까울수록 큼)
				Distance));

			TextItem.bCentreX = true;
			TextItem.bCentreY = true;

			Canvas->DrawItem(TextItem);
		}
	}
}

bool FFCSpawnerEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	USpawnerComponent* ClickedSpawner = nullptr;
	
	// 미리보기용 컴포넌트인지 확인. 커스텀 HitProxy가 아니라면 기본적으로 HActor 기반이다.
	const HActor* ActorProxy = HitProxyCast<HActor>(HitProxy);
	if (ActorProxy && ActorProxy->PrimComponent && SpawnerSubSystem->PreviewMap.Num() > 0)
	{
		TWeakObjectPtr<USpawnerComponent>* Spawner = SpawnerSubSystem->PreviewMap.Find(ActorProxy->PrimComponent);
		if (Spawner && Spawner->IsValid())
		{
			ClickedSpawner = Spawner->Get();
		}
	}

	// HitProxy가 ComponentVisProxy 인 경우
	if (ClickedSpawner == nullptr)
	{
		// 캐스팅 체크
		const HComponentVisProxy* VisProxy = HitProxyCast<HComponentVisProxy>(HitProxy);
		if (VisProxy && VisProxy->Component.IsValid())
		{
			ClickedSpawner = const_cast<USpawnerComponent*>(Cast<USpawnerComponent>(VisProxy->Component.Get()));
		}
	}

	if (ClickedSpawner)
	{
		// 우클릭 / 좌클릭
		if (Click.GetKey() == EKeys::RightMouseButton)
		{
			// 기존에 선택한 Spawner가 아닌 경우
			if (!ClickedSpawner->IsSelectedInEdMode())
			{
				DeSelectAll();
				SelectSpawner(ClickedSpawner, true);
			}
			// 컨텍스트 메뉴 생성
			if (const TSharedPtr<SWidget> MenuWidget = GenerateContextMenu())
			{
				FSlateApplication::Get().PushMenu(
					Owner->GetToolkitHost()->GetParentWidget(),
					FWidgetPath(),
					MenuWidget.ToSharedRef(),
					FSlateApplication::Get().GetCursorPos(),
					FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));
			}
		}
		else
		{
			// 상태 저장
			ClickedSpawner->Modify();

			if (Click.IsControlDown())
			{
				ClickedSpawner->IsSelectedInEdMode() ? SelectSpawner(ClickedSpawner, false) : SelectSpawner(ClickedSpawner, true);;
			}
			else if (Click.IsShiftDown())
			{
				SelectSpawner(ClickedSpawner, true);
			}
			else
			{
				// 기존 선택 해제 후 클릭한 Spawner 선택
				DeSelectAll();
				SelectSpawner(ClickedSpawner, true);
			}
		}
		return true;
	}

	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}

TSharedPtr<SWidget> FFCSpawnerEdMode::GenerateContextMenu() const
{
	TArray<UObject*> SelectedSpawners;
	if (SpawnerSubSystem->GetSelectedSpawners(SelectedSpawners))
	{
		return TSharedPtr<SWidget>();
	}

	// 커스텀 DetailsView 생성
	FPropertyEditorModule& PropEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs Args;
	Args.bAllowSearch = false;
	Args.bHideSelectionTip = false;
	Args.bShowScrollBar = false;
	Args.bShowOptions = false;

	// 선택된 SpawnerComponent의 DetailsView 생성
	TSharedRef<IDetailsView> DetailsView = PropEditorModule.CreateDetailView(Args);

	// Spawner 카테고리로 필터링. Transform은 IDetailCustomization 으로 특수 변형되었으므로 필터링 불가. SetObject 이전에 설정해야 정상 작동
	DetailsView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateLambda([](const FPropertyAndParent& PropertyAndParent)
	{
		const FProperty& Property = PropertyAndParent.Property;
		const FString& PropertyName = Property.GetMetaData(TEXT("Category"));
		// Spawner 카테고리만 통과
		return PropertyName.Equals(TEXT("Spawner"));
	}));
	DetailsView->SetObjects(SelectedSpawners);

	FMenuBuilder MenuBuilder(true, nullptr);

	MenuBuilder.AddWidget(
		SNew(SHorizontalBox)

		// Spawner 리셋 버튼 추가
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5, 0, 0, 0)
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Reset")))
			.ToolTipText(FText::FromString(TEXT("선택된 Spawner의 상태를 초기값으로 리셋")))
			.OnClicked_Lambda([SelectedSpawners]()
			{
				FScopedTransaction Transaction(FText::FromString(TEXT("선택된 모든 Spawner 상태 초기화")));
				for (auto& Selected : SelectedSpawners)
				{
					USpawnerComponent* Spawner = Cast<USpawnerComponent>(Selected);
					if (IsValid(Spawner))
					{
						Spawner->Modify();
						Spawner->Reset();
					}
				}
				return FReply::Handled();
			})
		]

		// 선택 해제 버튼 추가
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2, 0, 0, 0)
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Deselect")))
			.ToolTipText(FText::FromString(TEXT("선택된 모든 Spawner 선택 해제")))
			.OnClicked_Lambda([SelectedSpawners]()
			{
				FScopedTransaction Transaction(FText::FromString(TEXT("선택된 모든 Spawner 선택 해제")));
				for (auto Selected : SelectedSpawners)
				{
					USpawnerComponent* SelectedSpawner = Cast<USpawnerComponent>(Selected);
					SelectedSpawner->Modify();
					SelectedSpawner->MarkSelectInEdMode(false);
				}
				return FReply::Handled();
			})
		],

		FText::GetEmpty(),
		true
	);

	// 메뉴 구분선 추가
	MenuBuilder.AddMenuSeparator();

	// DetailsView 추가
	MenuBuilder.AddWidget(DetailsView, FText::GetEmpty(), true);

	return MenuBuilder.MakeWidget();
}

void FFCSpawnerEdMode::SelectSpawner(const TWeakObjectPtr<UObject>& InSpawner, bool bIsSelected) const
{
	// 트랜색션
	FScopedTransaction Transaction(FText::FromString("Select Spawner"));

	if (!InSpawner.IsValid()) return;
	USpawnerComponent* Spawner = Cast<USpawnerComponent>(InSpawner.Get());
	Spawner->Modify();

	if (bIsSelected)
	{
		Spawner->MarkSelectInEdMode(true);
	}
	else
	{
		Spawner->MarkSelectInEdMode(false);
	}

	TArray<UObject*> SelectedSpawners;
	SpawnerSubSystem->GetSelectedSpawners(SelectedSpawners);
	// 툴킷에 선택된 Spawner 목록 전달 (디테일 뷰 업데이트)
	if (FFCSpawnerModeToolkit* SpawnerToolkit = StaticCast<FFCSpawnerModeToolkit*>(Toolkit.Get()))
	{
		SpawnerToolkit->GetSpawnerDetailView()->SetObjects(SelectedSpawners);
	}
}

void FFCSpawnerEdMode::DeSelectAll() const
{
	// 트랜색션
	FScopedTransaction Transaction(FText::FromString(TEXT("Reset Selection")));

	TArray<UObject*> SelectedSpawners;
	SpawnerSubSystem->GetSelectedSpawners(SelectedSpawners);
	for (auto& Selected : SelectedSpawners)
	{
		USpawnerComponent* SelectedSpawner = Cast<USpawnerComponent>(Selected);
		SelectedSpawner->Modify();
		SelectedSpawner->MarkSelectInEdMode(false);
	}

	// 툴킷 디테일 뷰 리셋
	if (FFCSpawnerModeToolkit* SpawnerToolkit = StaticCast<FFCSpawnerModeToolkit*>(Toolkit.Get()))
	{
		SpawnerToolkit->GetSpawnerDetailView()->SetObject(nullptr);
	}
}
