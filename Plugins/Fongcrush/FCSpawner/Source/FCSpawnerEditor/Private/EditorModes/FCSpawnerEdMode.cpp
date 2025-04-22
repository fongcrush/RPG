// Fill out your copyright notice in the Description page of Project Settings.
#include "EditorModes/FCSpawnerEdMode.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "ComponentVisualizer.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "Selection.h"
#include "UnrealEdGlobals.h"
#include "Actors/SpawnerPreviewActor.h"
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
}

void FFCSpawnerEdMode::Enter()
{
	FEdMode::Enter();

	/** USelection Select Event 함수를 FFCSpawnerComponentVisualizer 에서 FFCSpawnerEdMode 것으로 교체 */

	FComponentVisualizer* ComponentVisualizer = GUnrealEd->FindComponentVisualizer(USpawnerComponent::StaticClass()->GetFName()).Get();
	check(ComponentVisualizer)

	FFCSpawnerComponentVisualizer* SpawnerVisualizer = StaticCast<FFCSpawnerComponentVisualizer*>(ComponentVisualizer);

	SpawnerVisualizer->UnRegisterBindSelectionEvents();
	RegisterSelectionEvent();

	// Toolkit 생성
	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShared<FFCSpawnerModeToolkit>();
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FFCSpawnerEdMode::Exit()
{
	/** USelection Select Event 함수를 FFCSpawnerEdMode 에서 FFCSpawnerComponentVisualizer 것으로 복원 */

	FComponentVisualizer* ComponentVisualizer = GUnrealEd->FindComponentVisualizer(USpawnerComponent::StaticClass()->GetFName()).Get();
	FFCSpawnerComponentVisualizer* SpawnerVisualizer = StaticCast<FFCSpawnerComponentVisualizer*>(ComponentVisualizer);

	SpawnerVisualizer->RegisterSelectionEvent();
	UnRegisterBindSelectionEvents();

	// 툴킷 종료
	FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
	Toolkit.Reset();
}

void FFCSpawnerEdMode::RegisterSelectionEvent()
{
	OnSelectedHandle = USelection::SelectObjectEvent.AddSP(this, &FFCSpawnerEdMode::OnSelected);
}

void FFCSpawnerEdMode::UnRegisterBindSelectionEvents()
{
	USelection::SelectObjectEvent.Remove(OnSelectedHandle);
}

void FFCSpawnerEdMode::OnSelected(UObject* Object)
{
	if (ASpawnerPreviewActor* PreviewActor = Cast<ASpawnerPreviewActor>(Object))
	{
		// 미리보기 엑터 선택 해제 후 Spawner 선택
		GEditor->SelectActor(PreviewActor, false, true, true);
		SelectSpawner(PreviewActor->GetOwnSpawner(), true);
	}
}

void FFCSpawnerEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);

	RemoveInvalidSelectedRefs();

	for (const auto& Spawner : GetWorld()->GetSubsystem<USpawnerSubSystem>()->GetSpawnerComps())
	{
		const FVector SpawnerLocation = Spawner->GetComponentLocation();
		const float Distance = FVector::Dist(View->ViewLocation, SpawnerLocation);
		const float& MaxDrawDistance = UFCSpawnerSettings::GetMaxDebugDrawDistance();

		// 설정의 최대 거리 값보다 가까워야 통과
		if (Distance > MaxDrawDistance)
		{
			return;
		}

		// 시각화 그리기 시작
		PDI->SetHitProxy(new HComponentVisProxy(Spawner));

		// 부모 엑터와의 연결 선
		PDI->DrawLine(Spawner->GetOwner()->GetActorLocation(), Spawner->GetComponentLocation(), FColor::Orange, SDPG_Foreground, 1.f);

		const FColor BottomColor = SelectedSpawners.Contains(Spawner) ? FColor::Yellow : Spawner->GetSpawnActorClass() ? FColor::Green : FColor::Red;
		const UMaterial* BottomMaterial = SelectedSpawners.Contains(Spawner) ? SelectedMaterial : Spawner->GetSpawnActorClass() ? ValidMaterial : InvalidMaterial;

		// 위치 표시용 점
		PDI->DrawPoint(SpawnerLocation, BottomColor, 10.f, SDPG_Foreground);

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
}

void FFCSpawnerEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);

	for (TActorIterator<ASpawnerPreviewActor> It(GetWorld()); It; ++It)
	{
		ASpawnerPreviewActor* PreviewActor = *It;

		USpawnerComponent* Spawner = PreviewActor->GetOwnSpawner();

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

			const UClass* SpawnerClass = PreviewActor->GetOwnSpawner()->GetSpawnActorClass();

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
}

bool FFCSpawnerEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	// 캐스팅 체크
	const HComponentVisProxy* VisProxy = StaticCast<HComponentVisProxy*>(HitProxy);
	if (!VisProxy || !VisProxy->Component.IsValid())
	{
		ResetSelection();
		return FEdMode::HandleClick(InViewportClient, HitProxy, Click);;
	}

	RemoveInvalidSelectedRefs();

	if (USpawnerComponent* ClickedSpawner = const_cast<USpawnerComponent*>(Cast<USpawnerComponent>(VisProxy->Component.Get())))
	{
		// 우클릭 / 좌클릭
		if (Click.GetKey() == EKeys::RightMouseButton)
		{
			// 기존에 선택한 Spawner가 아닌 경우
			if (!SelectedSpawners.Contains(ClickedSpawner))
			{
				SelectedSpawners.Empty();
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
			if (Click.IsControlDown())
			{
				SelectedSpawners.Contains(ClickedSpawner) ? SelectSpawner(ClickedSpawner, false) : SelectSpawner(ClickedSpawner);;
			}
			else if (Click.IsShiftDown())
			{
				SelectSpawner(ClickedSpawner);
			}
			else
			{
				// 기존 선택 해제 후 클릭한 Spawner 선택
				SelectedSpawners.Empty();
				SelectSpawner(ClickedSpawner, true);
			}
		}
		return true;
	}

	ResetSelection();
	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}

TSharedPtr<SWidget> FFCSpawnerEdMode::GenerateContextMenu() const
{
	RemoveInvalidSelectedRefs();

	if (SelectedSpawners.IsEmpty())
	{
		return nullptr;
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
		if (PropertyName.Equals(TEXT("Spawner")))
		{
			return true;
		}
		return false;
	}));
	DetailsView->SetObjects(SelectedSpawners.Array());

	FMenuBuilder MenuBuilder(true, nullptr);
	MenuBuilder.AddWidget(DetailsView, FText::GetEmpty(), true);

	return MenuBuilder.MakeWidget();
}

void FFCSpawnerEdMode::SelectSpawner(const TWeakObjectPtr<UObject>& Spawner, bool bIsSelected) const
{
	if (Spawner.IsValid())
	{
		bIsSelected ? (void)SelectedSpawners.Emplace(Spawner) : (void)SelectedSpawners.Remove(Spawner);
		
		FFCSpawnerModeToolkit* SpawnerToolkit = StaticCast<FFCSpawnerModeToolkit*>(Toolkit.Get());
		SpawnerToolkit->GetDetailView()->SetObjects(SelectedSpawners.Array());
	}
}

void FFCSpawnerEdMode::ResetSelection()
{	
	SelectedSpawners.Empty();
	
	FFCSpawnerModeToolkit* SpawnerToolkit = StaticCast<FFCSpawnerModeToolkit*>(Toolkit.Get());
	SpawnerToolkit->GetDetailView()->SetObject(nullptr);
}

void FFCSpawnerEdMode::RemoveInvalidSelectedRefs() const
{
	for (TSet<TWeakObjectPtr<UObject>>::TIterator It(SelectedSpawners); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
}
