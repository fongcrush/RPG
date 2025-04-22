// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleListeners/FCExampleEdModeListener.h"

#include "EditorModeRegistry.h"
#include "EditorModes/FCExampleEdMode.h"
#include "Styling/SlateStyleRegistry.h"


#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FFCExampleEdModeListener::StyleSet = nullptr;

void FFCExampleEdModeListener::OnStartupModule()
{
	RegisterStyleSet();
	RegisterEditorMode();
}

void FFCExampleEdModeListener::OnShutdownModule()
{
	UnregisterStyleSet();
	UnregisterEditorMode();
}

void FFCExampleEdModeListener::RegisterStyleSet()
{
	// Const icon sizes
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}
	
	const FString ResourcesDir = FPaths::ProjectPluginsDir() / TEXT("FC/FCEditors/Resources");
	
	// 스타일 세트 생성 및 사용 폴더 지정
	StyleSet = MakeShared<FSlateStyleSet>("FCEdModeStyle");
	StyleSet->SetContentRoot(ResourcesDir);
	StyleSet->SetCoreContentRoot(ResourcesDir);
	
	// 스플라인 에디터 아이콘 설정
	{
		StyleSet->Set("FCEdMode", new IMAGE_BRUSH(TEXT("IconFCEditorMode"), Icon40x40));
		StyleSet->Set("FCEdMode.Small", new IMAGE_BRUSH(TEXT("IconFCEditorMode"), Icon20x20));
	}
	
	// 스타일 세트 등록
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}


void FFCExampleEdModeListener::RegisterEditorMode()
{
	FEditorModeRegistry::Get().RegisterMode<FFCExampleEdMode>(
		FFCExampleEdMode::ModeID,
		FText::FromString(TEXT("퐁크러시")),
		FSlateIcon(StyleSet->GetStyleSetName(), "FCEdMode", "FCEdMode.Small"),
		true,
		500);
}

void FFCExampleEdModeListener::UnregisterStyleSet()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

void FFCExampleEdModeListener::UnregisterEditorMode()
{
	FEditorModeRegistry::Get().UnregisterMode(FFCExampleEdMode::ModeID);
}

#undef IMAGE_BRUSH