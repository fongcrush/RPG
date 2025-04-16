#include "Visualizers/SpawnerVisualizer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "FongcrushSettings.h"
#include "FongcrushSpawner.h"
#include "Components/SpawnerComponent.h"
#include "Editor/PropertyEditor/Private/CategoryPropertyNode.h"
#include "Editor/PropertyEditor/Private/PropertyNode.h"

void FSpawnerComponentVisualizer::OnRegister()
{
	BottomMaterial = LoadObject<UMaterial>(nullptr, TEXT("/FongcrushGame/M_DebugMat.M_DebugMat"));
	ValidMaterial = LoadObject<UMaterial>(nullptr, TEXT("/FongcrushGame/M_DebugMat2.M_DebugMat2"));
	InValidMaterial = LoadObject<UMaterial>(nullptr, TEXT("/FongcrushGame/M_DebugMat3.M_DebugMat3"));

	check(BottomMaterial)
	check(ValidMaterial)
	check(InValidMaterial)
}

TSharedPtr<SWidget> FSpawnerComponentVisualizer::GenerateContextMenu() const
{
	UActorComponent* Component = SelectedComponent.Get();
	if (!IsValid(Component)) return nullptr;

	FMenuBuilder MenuBuilder(true, nullptr);

	// 커스텀 DetailsView 생성
	FPropertyEditorModule& PropEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

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
	DetailsView->SetObject(Component);
	
	MenuBuilder.AddWidget(DetailsView, FText::GetEmpty(), true);
	return MenuBuilder.MakeWidget();
}

void FSpawnerComponentVisualizer::DrawVisualization(
	const UActorComponent* Component,
	const FSceneView* View,
	FPrimitiveDrawInterface* PDI)
{
	const USpawnerComponent* SpawnerComp = Cast<USpawnerComponent>(Component);
	
	const FVector SpawnerPos = SpawnerComp->GetComponentLocation();
	const float Distance = FVector::Dist(View->ViewLocation, SpawnerPos);
	const float& MaxDrawDistance = UFongcrushSettings::GetMaxDebugDrawDistance();

	// 설정의 최대 거리 값보다 가까워야 통과
	if (Distance > MaxDrawDistance) return;

	PDI->SetHitProxy(new HComponentVisProxy(SpawnerComp));
	// 부모 엑터에 디버깅 선 연결
	PDI->DrawLine(SpawnerComp->GetOwner()->GetActorLocation(), SpawnerComp->GetComponentLocation(), FColor::Orange, 30.f, SDPG_World);

	// 시각화용 도형
	{
		const UMaterial* UsedMaterial = SpawnerComp->GetSpawnActorClass() ? ValidMaterial : InValidMaterial;
		
		// 밑면
		DrawDisc(PDI,
		         SpawnerPos,
		         FVector::XAxisVector, FVector::YAxisVector,
		         FColor::White,
		         25.f, 32,
		         BottomMaterial->GetRenderProxy(),
		         SDPG_World);

		// 원툴
		const FMatrix ConeScaleMat = FScaleMatrix(100.f);
		const FMatrix ConeRotMat = FRotationMatrix(SpawnerComp->GetComponentRotation() + FRotator(90, 0, 0));
		const FMatrix ConePosMat = FTranslationMatrix(SpawnerPos);
		const FMatrix ArrowToWorld = ConeScaleMat * ConeRotMat * ConePosMat;
		//밑면 렌더링 안됨, 위아래 뒤집음
		DrawCone(PDI,
		         ArrowToWorld,
		         FMath::DegreesToRadians(20.f), FMath::DegreesToRadians(20.f),
		         32, false,
		         FLinearColor::White,
		         UsedMaterial->GetRenderProxy(),
		         SDPG_World);

		// 원뿔 밑면(위쪽 뚜껑)
		DrawDisc(PDI,
		         SpawnerPos + FVector(0.f, 0.f, 93.f),
		         FVector::XAxisVector, FVector::YAxisVector,
		         FColor::White,
		         34.f, 32,
		         UsedMaterial->GetRenderProxy(),
		         SDPG_World);
	}
	PDI->SetHitProxy(nullptr);
}

void FSpawnerComponentVisualizer::DrawVisualizationHUD(
	const UActorComponent* Component,
	const FViewport* Viewport,
	const FSceneView* View,
	FCanvas* Canvas)
{
	const USpawnerComponent* SpawnerComp = Cast<USpawnerComponent>(Component);

	const FVector SpawnerPos = SpawnerComp->GetComponentLocation();
	const float Distance = FVector::Dist(View->ViewLocation, SpawnerPos);
	const float& MaxDrawDistance = UFongcrushSettings::GetMaxDebugDrawDistance();

	// 설정의 최대 거리 값보다 가까워야 통과
	if (Distance > MaxDrawDistance) return;

	// 시야에 들어올 경우에만 통과
	if (!View->ViewFrustum.IntersectPoint(SpawnerPos)) return;

	FVector2D ScreenPos;
	if (View->WorldToPixel(SpawnerPos + FVector(0, 0, 50.f), ScreenPos))
	{
		ScreenPos /= Canvas->GetDPIScale();

		const UClass* SpawnerClass = SpawnerComp->GetSpawnActorClass();

		// 디버깅 문자열 초기화
		FString DebugString;
		if (SpawnerClass)
		{
			DebugString += FString::Printf(TEXT("Spawn : %s\n"), *SpawnerComp->GetSpawnActorClass()->GetName());
			DebugString += FString::Printf(TEXT("Spawn Delay: %.2fs"), SpawnerComp->GetSpawnTime());
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
			Distance
		));
		TextItem.bCentreX = true;
		TextItem.bCentreY = true;

		Canvas->DrawItem(TextItem);
	}
}

bool FSpawnerComponentVisualizer::VisProxyHandleClick(
	FEditorViewportClient* InViewportClient,
	HComponentVisProxy* VisProxy,
	const FViewportClick& Click)
{
	UE_LOG(FongcrushSpawner, Display, TEXT("VisProxyHandleClick"));
	SelectedComponent = const_cast<USpawnerComponent*>(Cast<USpawnerComponent>(VisProxy->Component.Get()));
	return true;
}

void FSpawnerComponentVisualizer::EndEditing()
{
	UE_LOG(FongcrushSpawner, Display, TEXT("EndEditing"));
}
