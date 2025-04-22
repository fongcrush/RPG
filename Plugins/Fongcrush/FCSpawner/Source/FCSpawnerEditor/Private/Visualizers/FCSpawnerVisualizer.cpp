#include "Visualizers/FCSpawnerVisualizer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "EditorModeManager.h"
#include "Selection.h"
#include "FCSpawnerEditor.h"
#include "Actors/SpawnerPreviewActor.h"
#include "Settings/FCSpawnerSettings.h"
#include "Components/SpawnerComponent.h"
#include "EditorModes/FCSpawnerEdMode.h"

void FFCSpawnerComponentVisualizer::OnRegister()
{
	ValidMaterial = LoadObject<UMaterial>(nullptr, TEXT("/FCSpawner/M_Debug_Valid.M_Debug_Valid"));
	check(ValidMaterial)
	ValidMaterial2 = LoadObject<UMaterial>(nullptr, TEXT("/FCSpawner/M_Debug_Valid_2.M_Debug_Valid_2"));
	check(ValidMaterial2)
	InvalidMaterial = LoadObject<UMaterial>(nullptr, TEXT("/FCSpawner/M_Debug_Invalid.M_Debug_Invalid"));
	check(InvalidMaterial)
	InvalidMaterial2 = LoadObject<UMaterial>(nullptr, TEXT("/FCSpawner/M_Debug_Invalid_2.M_Debug_Invalid_2"));
	check(InvalidMaterial2)
	SelectedMaterial = LoadObject<UMaterial>(nullptr, TEXT("/FCSpawner/M_Debug_Selected.M_Debug_Selected"));
	check(SelectedMaterial)
	SelectedMaterial2 = LoadObject<UMaterial>(nullptr, TEXT("/FCSpawner/M_Debug_Selected_2.M_Debug_Selected_2"));
	check(SelectedMaterial2)

	RegisterSelectionEvent();
}

void FFCSpawnerComponentVisualizer::RegisterSelectionEvent()
{
	OnSelectedHandle = USelection::SelectObjectEvent.AddSP(this, &FFCSpawnerComponentVisualizer::OnSelected);
	// USelection::SelectionChangedEvent.AddStatic(&FFCSpawnerComponentVisualizer::OnSelected);
}

void FFCSpawnerComponentVisualizer::UnRegisterBindSelectionEvents()
{
	USelection::SelectObjectEvent.Remove(OnSelectedHandle);
}

void FFCSpawnerComponentVisualizer::OnSelected(UObject* Object)
{
	if (ASpawnerPreviewActor* PreviewActor = Cast<ASpawnerPreviewActor>(Object);
		PreviewActor && PreviewActor->IsSelected())
	{
		// 미리보기 엑터 선택 해제 후 Spawner 선택
		GEditor->SelectActor(PreviewActor, false, true, true);
		GEditor->SelectComponent(PreviewActor->GetOwnSpawner(), true, true, true);
	}
}

void FFCSpawnerComponentVisualizer::DrawVisualization(
	const UActorComponent* Component,
	const FSceneView* View,
	FPrimitiveDrawInterface* PDI)
{
	// 왜인지 모르겠지만 Component가 nullptr일 경우가 있음
	if (!IsValid(Component)) return;

	// 현재 SpawnerEdMode일 경우 맡기고 종료 
	if (GLevelEditorModeTools().IsModeActive(FFCSpawnerEdMode::ModeID)) return;

	const USpawnerComponent* Spawner = Cast<USpawnerComponent>(Component);

	const FVector SpawnerLocation = Spawner->GetComponentLocation();
	const float Distance = FVector::Dist(View->ViewLocation, SpawnerLocation);
	const float& MaxDrawDistance = UFCSpawnerSettings::GetMaxDebugDrawDistance();

	// 설정의 최대 거리 값보다 가까워야 통과
	if (Distance > MaxDrawDistance) return;

	// 시각화 그리기 시작
	PDI->SetHitProxy(new HComponentVisProxy(Spawner));

	// 부모 엑터와의 연결 선
	PDI->DrawLine(Spawner->GetOwner()->GetActorLocation(), Spawner->GetComponentLocation(), FColor::Orange, SDPG_World, 1.f);

	// 선택된 Spawner 목록
	TArray<UObject*> SelectedSpawners;
	GEditor->GetSelectedComponents()->GetSelectedObjects(USpawnerComponent::StaticClass(), SelectedSpawners);

	const FColor BottomColor = SelectedSpawners.Contains(Spawner) ? FColor::Yellow : Spawner->GetSpawnActorClass() ? FColor::Green : FColor::Red;
	const UMaterial* BottomMaterial = SelectedSpawners.Contains(Spawner) ? SelectedMaterial : Spawner->GetSpawnActorClass() ? ValidMaterial : InvalidMaterial;

	// 위치 표시용 점
	PDI->DrawPoint(SpawnerLocation, BottomColor, 10.f, SDPG_World);

	// 바닥 구분용 원
	DrawDisc(PDI,
	         SpawnerLocation,
	         FVector::XAxisVector, FVector::YAxisVector,
	         BottomColor,
	         25.f, 32,
	         BottomMaterial->GetRenderProxy(),
	         SDPG_World);

	// 생성할 엑터 클래스가 없을 경우
	if (Spawner->GetSpawnActorClass() == nullptr)
	{
		const FMatrix ConeScaleMat = FScaleMatrix(100.f);
		const FMatrix ConeRotMat = FRotationMatrix(Spawner->GetComponentRotation() + FRotator(90, 0, 0));
		const FMatrix ConePosMat = FTranslationMatrix(SpawnerLocation);
		const FMatrix ArrowToWorld = ConeScaleMat * ConeRotMat * ConePosMat;

		const UMaterial* ConeMaterial = SelectedSpawners.Contains(Spawner) ? SelectedMaterial2 : InvalidMaterial2;

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
	if (!IsValid(Component)) return;

	// 현재 SpawnerEdMode일 경우 맡기고 종료 
	if (GLevelEditorModeTools().IsModeActive(FFCSpawnerEdMode::ModeID)) return;

	const USpawnerComponent* Spawner = Cast<USpawnerComponent>(Component);

	const FVector SpawnerLocation = Spawner->GetComponentLocation();
	const float Distance = FVector::Dist(View->ViewLocation, SpawnerLocation);
	const float& MaxDrawDistance = UFCSpawnerSettings::GetMaxDebugDrawDistance();

	// 설정의 최대 거리 값보다 가까워야 통과
	if (Distance > MaxDrawDistance) return;

	// 시야에 들어올 경우에만 통과
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

		USpawnerComponent* ClickedSpawner = const_cast<USpawnerComponent*>(Cast<USpawnerComponent>(ComponentVisProxy->Component.Get()));

		if (Click.IsControlDown())
		{
			TArray<UObject*> SelectedSpawners;
			GEditor->GetSelectedComponents()->GetSelectedObjects(USpawnerComponent::StaticClass(), SelectedSpawners);

			if (SelectedSpawners.Contains(ClickedSpawner))
			{
				GEditor->SelectComponent(ClickedSpawner, false, true);
			}
			else
			{
				GEditor->SelectComponent(ClickedSpawner, true, true);
			}
		}
		else if (Click.IsShiftDown())
		{
			GEditor->SelectComponent(ClickedSpawner, true, true);
		}
		return true;
	}
	return false;
}

bool FFCSpawnerComponentVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	// 호출부에서 안전한 HitProxy 캐스팅 보장

	if (!VisProxy->Component.IsValid()) return false;

	// 현재 SpawnerEdMode일 경우 맡기고 종료
	if (GLevelEditorModeTools().IsModeActive(FFCSpawnerEdMode::ModeID)) return false;

	// 선택된 Spawner 목록
	TArray<UObject*> SelectedSpawners;
	GEditor->GetSelectedComponents()->GetSelectedObjects(USpawnerComponent::StaticClass(), SelectedSpawners);

	USpawnerComponent* ClickedSpawner = const_cast<USpawnerComponent*>(Cast<USpawnerComponent>(VisProxy->Component.Get()));

	// 우클릭 / 좌클릭 분기
	if (Click.GetKey() == EKeys::RightMouseButton)
	{
		// 선택된 Spawner가 아닌 경우
		if (!SelectedSpawners.Contains(ClickedSpawner))
		{
			GEditor->SelectNone(true, true);
			GEditor->SelectComponent(ClickedSpawner, true, true);
		}
	}
	else
	{
		// 기존 선택 취소 후 클릭한 Spawner 선택
		GEditor->SelectNone(true, true);
		GEditor->SelectComponent(ClickedSpawner, true, true);
	}
	return true;
}

TSharedPtr<SWidget> FFCSpawnerComponentVisualizer::GenerateContextMenu() const
{
	// 호출부에서 안전한 HitProxy 캐스팅 보장

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
	TArray<UObject*> SelectedSpawners;
	GEditor->GetSelectedComponents()->GetSelectedObjects(USpawnerComponent::StaticClass(), SelectedSpawners);

	DetailsView->SetObjects(SelectedSpawners);

	FMenuBuilder MenuBuilder(true, nullptr);
	MenuBuilder.AddWidget(DetailsView, FText::GetEmpty(), true);

	return MenuBuilder.MakeWidget();
}
