// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleListeners/FongcrushEditorModeListener.h"

#include "EditorModeRegistry.h"
#include "EditorModes/FongcrushEditorMode.h"
#include "Styling/SlateStyleRegistry.h"


#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FFongcrushEditorModeListener::StyleSet = nullptr;

void FFongcrushEditorModeListener::OnStartupModule()
{
	RegisterStyleSet();
	RegisterEditorMode();
}

void FFongcrushEditorModeListener::OnShutdownModule()
{
	UnregisterStyleSet();
	UnregisterEditorMode();
}

void FFongcrushEditorModeListener::RegisterStyleSet()
{
	// Const icon sizes
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}
	
	const FString ResourcesDir = FPaths::ProjectPluginsDir() / TEXT("Fongcrush/FongcrushEditors/Resources");
	
	// 스타일 세트 생성 및 사용 폴더 지정
	StyleSet = MakeShared<FSlateStyleSet>("FongcrushEdModeStyle");
	StyleSet->SetContentRoot(ResourcesDir);
	StyleSet->SetCoreContentRoot(ResourcesDir);
	
	// 스플라인 에디터 아이콘 설정
	{
		StyleSet->Set("FongcrushEdMode", new IMAGE_BRUSH(TEXT("IconFongcrushEditorMode"), Icon40x40));
		StyleSet->Set("FongcrushEdMode.Small", new IMAGE_BRUSH(TEXT("IconFongcrushEditorMode"), Icon20x20));
	}
	
	// 스타일 세트 등록
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}


void FFongcrushEditorModeListener::RegisterEditorMode()
{
	FEditorModeRegistry::Get().RegisterMode<FFongcrushEditorMode>(
		FFongcrushEditorMode::ModeID,
		FText::FromString(TEXT("퐁크러시")),
		FSlateIcon(StyleSet->GetStyleSetName(), "FongcrushEdMode", "FongcrushEdMode.Small"),
		true,
		500);
}

void FFongcrushEditorModeListener::UnregisterStyleSet()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

void FFongcrushEditorModeListener::UnregisterEditorMode()
{
	FEditorModeRegistry::Get().UnregisterMode(FFongcrushEditorMode::ModeID);
}

#undef IMAGE_BRUSH