// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FongcrushTestFactory.h"
#include "FongcrushImportFactory.generated.h"

class UFongcrushTestData;

/**
 * 
 */
UCLASS()
class FONGCRUSHTESTEDITOR_API UFongcrushImportFactory : public UFongcrushTestFactory
{
	GENERATED_BODY()

public:
	UFongcrushImportFactory();
	
	/** 에셋 인스턴스 생성 및 텍스트 파일에서 데이터 불러와서 저장 */
	virtual UObject* FactoryCreateText(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		const TCHAR* Type,
		const TCHAR*& Buffer,
		const TCHAR* BufferEnd,
		FFeedbackContext* Warn) override;

	/** 파일명이 지원하는 확장자를 포함하면 임포트 가능 */
	virtual bool FactoryCanImport(const FString& Filename) override;

	/** 가져오기 한 데이터 에셋에 설정 */
	static void SetFongcrushDataFromText(UFongcrushTestData* Data, const TCHAR*& Buffer, const TCHAR* BufferEnd);
};
