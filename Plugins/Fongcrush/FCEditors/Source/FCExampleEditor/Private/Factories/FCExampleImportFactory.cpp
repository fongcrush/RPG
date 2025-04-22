// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/FCExampleImportFactory.h"
#include "Objects/FCExampleData.h"
#include "EditorFramework/AssetImportData.h"

UFCExampleImportFactory::UFCExampleImportFactory()
{	
    Formats.Emplace(TEXT("xmp;FC Data")); // 지원하는 파일 확장자 등록
	bCreateNew = false; // true면 엔진 자체 생성 전용이라 외부 파일 Import 불가능,  false면 외부 파일 Import 전용(에디터에서 생성 불가)
	bEditAfterNew = false; // 새로 만든 후 에디터 열기 여부
	bEditorImport = true;
	bText = true;
}

UObject* UFCExampleImportFactory::FactoryCreateText(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	const TCHAR* Type,
	const TCHAR*& Buffer,
	const TCHAR* BufferEnd,
	FFeedbackContext* Warn)
{
	// PreImport
	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	// 클래스 타입이나 파일 확장자가 다르면 패스
	if (InClass != UFCExampleData::StaticClass() ||	FCString::Stricmp(Type, TEXT("xmp")) != 0)
		return nullptr;

	// 새 오브젝트 생성
	UFCExampleData* Data = CastChecked<UFCExampleData>(NewObject<UFCExampleData>(InParent, InName, Flags));
	// 데이터 불러오기
	SetFCDataFromText(Data, Buffer, BufferEnd);

	// 나중에 다시 가져올수 있도록 가져온 파일의 상대 경로 저장
	Data->SourceFilePath = UAssetImportData::SanitizeImportFilename(CurrentFilename, Data->GetPackage());

	// PostImport
	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.Broadcast(this, Data);
	
	return Data;
}

bool UFCExampleImportFactory::FactoryCanImport(const FString& Filename)
{
	return FPaths::GetExtension(Filename).Equals(TEXT("xmp"));
}

void UFCExampleImportFactory::SetFCDataFromText(UFCExampleData* Data, const TCHAR*& Buffer, const TCHAR* BufferEnd)
{
	Data->TestString = Buffer;
}
