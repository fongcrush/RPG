#include "Visualizers/FCSpawnerVisualizer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "EditorModeManager.h"
#include "FCSpawnerEditor.h"
#include "Selection.h"
#include "Settings/FCSpawnerSettings.h"
#include "Components/SpawnerComponent.h"
#include "EditorModes/FCSpawnerEdMode.h"

void FFCSpawnerComponentVisualizer::OnRegister()
{
	UE_LOG(FCSpawnerEditor, Display, TEXT("SpawnerComponentVisualize 등록"))

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
	
	UnRegisterSelectionChangedEvent();
	RegisterSelectionChangedEvent();
}

void FFCSpawnerComponentVisualizer::RegisterSelectionChangedEvent()
{
	OnSelectedHandle = USelection::SelectionChangedEvent.AddSPLambda(this, [&](UObject* const& Object)
	{
		if (!IsValid(Object)) return;

		// USelection::SelectionChangedEvent 의 파라미터는 선택된 오브젝트 자체가 아닌, USelection 객체임
		USelection* Selection = Cast<USelection>(Object);

		TArray<UObject*> PreviousSelectedSpawners = SelectedSpawners;
		if (Selection->GetSelectedObjects(USpawnerComponent::StaticClass(), SelectedSpawners) > 0)
		{
			for (auto& SelectedObject : SelectedSpawners)
			{
				USpawnerComponent* SelectedSpawner = Cast<USpawnerComponent>(SelectedObject);
				if (SelectedSpawner == nullptr) continue;

				for (const auto& PreviewComp : SelectedSpawner->PreviewComponents)
				{
					if (!IsValid(PreviewComp)) continue;
					PreviewComp->SetRenderCustomDepth(true); // 윤곽선 설정
				}
			}
		}
		else
		{
			// 선택된 Spawner가 전혀 없을 경우 모든 윤곽선 해제
			for (auto& PrevSelected : PreviousSelectedSpawners)
			{
				USpawnerComponent* PreviousSpawner = Cast<USpawnerComponent>(PrevSelected);
				if (PreviousSpawner == nullptr) continue;

				for (const auto& PreviewComp : PreviousSpawner->PreviewComponents)
				{
					if (!IsValid(PreviewComp)) continue;
					PreviewComp->SetRenderCustomDepth(false); // 윤곽선 해제
				}
			}
		}
	});
}

void FFCSpawnerComponentVisualizer::UnRegisterSelectionChangedEvent()
{
	USelection::SelectionChangedEvent.Remove(OnSelectedHandle);

	// 모든 윤곽선 비활성화
	for (auto& Selected : SelectedSpawners)
	{
		USpawnerComponent* SelectedSpawner = Cast<USpawnerComponent>(Selected);
		if (SelectedSpawner == nullptr) continue;

		for (const auto& PreviewComp : SelectedSpawner->PreviewComponents)
		{
			if (!IsValid(PreviewComp)) continue;
			PreviewComp->SetRenderCustomDepth(false); // 윤곽선 해제
		}
	}
}

void FFCSpawnerComponentVisualizer::DrawVisualization(
	const UActorComponent* Component,
	const FSceneView* View,
	FPrimitiveDrawInterface* PDI)
{
	// 왜인지 모르겠지만 Component가 nullptr이거나 클래스가 다른 경우가 있음
	const USpawnerComponent* Spawner = Cast<USpawnerComponent>(Component);
	if (!IsValid(Spawner)) return;

	// 현재 SpawnerEdMode일 경우 맡기고 종료 
	if (GLevelEditorModeTools().IsModeActive(FFCSpawnerEdMode::ModeID)) return;

	const FVector SpawnerLocation = Spawner->GetComponentLocation();
	const float Distance = FVector::Dist(View->ViewLocation, SpawnerLocation);
	const float MaxDrawDistance = UFCSpawnerSettings::GetMaxDebugDrawDistance();

	// 설정의 최대 거리 값보다 가까워야 통과
	if (Distance > MaxDrawDistance) return;

	// 시각화 그리기 시작
	PDI->SetHitProxy(new HComponentVisProxy(Spawner));
	{
		GEditor->GetSelectedComponents()->GetSelectedObjects(USpawnerComponent::StaticClass(), SelectedSpawners);

		const FColor DebugColor = Spawner->IsSelected() ? FColor::Yellow : (Spawner->GetSpawnActorClass() ? FColor::White : FColor::Red);

		// 위치 표시용 점
		PDI->DrawPoint(SpawnerLocation, DebugColor, 15.f, SDPG_Foreground);

		// 부모 엑터와의 연결 선
		PDI->DrawLine(Spawner->GetOwner()->GetActorLocation(),
		              Spawner->GetComponentLocation(),
		              Spawner->IsSelected() ? FColor::Orange : FColor::White,
		              SDPG_Foreground,
		              1.f);

		// 생성할 엑터 클래스가 없을 경우
		if (Spawner->GetSpawnActorClass() == nullptr)
		{
			const FMatrix ConeScaleMat = FScaleMatrix(100.f);
			const FMatrix ConeRotMat = FRotationMatrix(Spawner->GetComponentRotation() + FRotator(90, 0, 0));
			const FMatrix ConePosMat = FTranslationMatrix(SpawnerLocation);
			const FMatrix ConeToWorld = ConeScaleMat * ConeRotMat * ConePosMat;
			const UMaterialInterface* ConeMaterial = Spawner->IsSelected() ? SelectedMaterial2 : InvalidMaterial2;

			// 원뿔 옆면 (DrawCone은 밑면 렌더링 X)
			DrawCone(PDI,
			         ConeToWorld,
			         FMath::DegreesToRadians(20.f), FMath::DegreesToRadians(20.f),
			         32, false,
			         DebugColor,
			         ConeMaterial->GetRenderProxy(),
			         SDPG_World);

			// 원뿔 밑면(위쪽 뚜껑)
			DrawDisc(PDI,
			         SpawnerLocation + FVector(0.f, 0.f, 93.f),
			         FVector::XAxisVector, FVector::YAxisVector,
			         DebugColor,
			         34.f, 32,
			         ConeMaterial->GetRenderProxy(),
			         SDPG_World);

			const UMaterialInterface* BottomMaterial = Spawner->IsSelected() ? SelectedMaterial : InvalidMaterial;

			// 바닥 구분용 원
			DrawDisc(PDI,
			         SpawnerLocation,
			         FVector::XAxisVector, FVector::YAxisVector,
			         DebugColor,
			         25.f, 32,
			         BottomMaterial->GetRenderProxy(),
			         SDPG_World);
		}
		else if (Spawner->IsSelected())
		{
			for (auto& PreviewComp : Spawner->PreviewComponents)
			{
				// 윤곽선 설정
				PreviewComp->SetRenderCustomDepth(true);
			}
		}
		else
		{			
			for (auto& PreviewComp : Spawner->PreviewComponents)
			{
				// 윤곽선 해제
				PreviewComp->SetRenderCustomDepth(false);
			}
		}
	}
	// 시각화 그리기 종료
	PDI->SetHitProxy(nullptr);
}

void FFCSpawnerComponentVisualizer::DrawVisualizationHUD(
	const UActorComponent* Component,
	const FViewport* Viewport,
	const FSceneView* View,
	FCanvas* Canvas)
{
	const USpawnerComponent* Spawner = Cast<USpawnerComponent>(Component);
	if (!IsValid(Spawner)) return;

	// 현재 SpawnerEdMode일 경우 맡기고 종료 
	if (GLevelEditorModeTools().IsModeActive(FFCSpawnerEdMode::ModeID)) return;

	const FVector SpawnerLocation = Spawner->GetComponentLocation();
	const float Distance = FVector::Dist(View->ViewLocation, SpawnerLocation);
	const float& MaxDrawDistance = UFCSpawnerSettings::GetMaxDebugDrawDistance();

	// 설정의 최대 거리 값보다 가까워야 통과
	if (Distance > MaxDrawDistance) return;

	// 최대 거리에 들어올 경우에만 통과
	if (!View->ViewFrustum.IntersectPoint(SpawnerLocation)) return;

	FVector2D ScreenPos;
	if (View->WorldToPixel(SpawnerLocation + FVector(0, 0, 50.f), ScreenPos))
	{
		ScreenPos /= Canvas->GetDPIScale();

		const UClass* SpawnerClass = Spawner->GetSpawnActorClass();

		// 디버깅 문자열 초기화
		FString DebugString;
		if (SpawnerClass)
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

bool FFCSpawnerComponentVisualizer::HandleModifiedClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	if (HComponentVisProxy* ComponentVisProxy = HitProxyCast<HComponentVisProxy>(HitProxy))
	{
		if (!ComponentVisProxy->Component.IsValid()) return false;

		// 현재 SpawnerEdMode일 경우 맡기고 종료 
		if (GLevelEditorModeTools().IsModeActive(FFCSpawnerEdMode::ModeID)) return false;

		USpawnerComponent* Spawner = const_cast<USpawnerComponent*>(Cast<USpawnerComponent>(ComponentVisProxy->Component.Get()));

		if (Click.IsControlDown())
		{
			if (Spawner->IsSelected())
			{
				// 선택된 Spawner인 경우 선택 취소
				GEditor->SelectComponent(Spawner, false, true);
			}
			else
			{
				// 선택된 Spawner가 아닌 경우 선택 
				GEditor->SelectComponent(Spawner, true, true);
			}
		}
		else if (Click.IsShiftDown())
		{
			GEditor->SelectComponent(Spawner, true, true);
		}
		return true;
	}

	return false;
}

bool FFCSpawnerComponentVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	// 호출부에서 HitProxy 캐스팅 보장 됨

	if (!VisProxy->Component.IsValid()) return false;

	// 현재 SpawnerEdMode일 경우 맡기고 종료
	if (GLevelEditorModeTools().IsModeActive(FFCSpawnerEdMode::ModeID)) return false;

	USpawnerComponent* Spawner = const_cast<USpawnerComponent*>(Cast<USpawnerComponent>(VisProxy->Component.Get()));

	if (Click.GetKey() == EKeys::RightMouseButton || Spawner->IsSelected())
	{
		// GenerateContextMenu 에서 처리하므로 여기서는 아무것도 하지 않음
		return true;
	}

	// 기존 선택 취소 후 클릭한 Spawner 선택
	GEditor->SelectNone(true, true);
	GEditor->SelectComponent(Spawner, true, true);

	return true;
}

TSharedPtr<SWidget> FFCSpawnerComponentVisualizer::GenerateContextMenu() const
{
	// 호출부에서 HitProxy 캐스팅 보장 됨

	// 현재 SpawnerEdMode일 경우 맡기고 종료
	if (GLevelEditorModeTools().IsModeActive(FFCSpawnerEdMode::ModeID)) return nullptr;

	// 커스텀 DetailsView 생성
	FPropertyEditorModule& PropEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// DetailsView 초기화용 설정 인자
	FDetailsViewArgs Args;
	Args.bAllowSearch = false;
	Args.bHideSelectionTip = false;
	Args.bShowScrollBar = false;
	Args.bShowOptions = false;

	TSharedRef<IDetailsView> DetailsView = PropEditorModule.CreateDetailView(Args);
	// Spawner 카테고리로 필터링. Transform은 IDetailCustomization 으로 특수 변형되었으므로 필터링 불가. SetObject 이전에 설정해야 정상 작동
	DetailsView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateLambda([](const FPropertyAndParent& PropertyAndParent)
	{
		const FProperty& Property = PropertyAndParent.Property;
		const FString& PropertyName = Property.GetMetaData(TEXT("Category"));
		if (PropertyName.Equals(TEXT("Spawner")))
		{
			return true;
		}
		return false;
	}));

	// 선택된 Spawner 목록
	TArray<UObject*> Selected;
	GEditor->GetSelectedComponents()->GetSelectedObjects(USpawnerComponent::StaticClass(), Selected);
	DetailsView->SetObjects(Selected);

	FMenuBuilder MenuBuilder(true, nullptr);
	MenuBuilder.AddWidget(DetailsView, FText::GetEmpty(), true);

	return MenuBuilder.MakeWidget();
}
