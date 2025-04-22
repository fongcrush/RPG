// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/FCExampleReImportFactory.h"
#include "Objects/FCExampleData.h"
#include "EditorFramework/AssetImportData.h"


bool UFCExampleReImportFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	if (const UFCExampleData* Data = Cast<UFCExampleData>(Obj))
	{
		// 패키지의 절대 경로 저장
		OutFilenames.Emplace(UAssetImportData::ResolveImportFilename(Data->SourceFilePath, Data->GetPackage()));
		return true;
	}
	return false;
}

void UFCExampleReImportFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UFCExampleData* Data = Cast<UFCExampleData>(Obj);
	if (Data && ensure(NewReimportPaths.Num() == 1))
	{
		// 오브젝트 패키지 경로에 따른 상대 경로 저장
		Data->SourceFilePath = UAssetImportData::SanitizeImportFilename(NewReimportPaths[0], Data->GetPackage());
	}
}

EReimportResult::Type UFCExampleReImportFactory::Reimport(UObject* Obj)
{
	UFCExampleData* Data = Cast<UFCExampleData>(Obj);
	if (!Data)
	{
		return EReimportResult::Failed;
	}
	
	const FString Filename = UAssetImportData::ResolveImportFilename(Data->SourceFilePath, Data->GetPackage());
	// 지원하는 확장자 파일이 아니면 실패
	if (!FPaths::GetExtension(Filename).Equals(TEXT("xmp")))
	{
		return EReimportResult::Failed;
	}
	
	CurrentFilename = Filename;
	FString StringData;
	// 파일을 문자열로 로드
	if (FFileHelper::LoadFileToString(StringData, *CurrentFilename))
	{
		// UE에 수정 요청 및 수정 사항 있다고 알림
		const TCHAR* Ptr = *StringData;
		Data->Modify();
		Data->MarkPackageDirty();

		// 데이터 변경
		SetFCDataFromText(Data, Ptr, Ptr + StringData.Len());

		// 상대 경로 저장
		Data->SourceFilePath = UAssetImportData::SanitizeImportFilename(CurrentFilename, Data->GetPackage());
	}

	return EReimportResult::Succeeded;
}
