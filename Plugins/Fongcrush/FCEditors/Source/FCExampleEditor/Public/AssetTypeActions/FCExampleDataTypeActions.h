// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetTypeActions_Base.h"

class UFCExampleData;

/**
 * FAssetTypeActions_Base - 사용자 정의 에셋에 대해 이 클래스를 파생하여 해당 에셋을 어떻게 표시할지와 콘텐츠 브라우저에서 우클릭 시 나오는 메뉴를 정의
 * 
 * AssetTools 모듈을 통해서 등록한다.
 * e.g.
 * 
 *		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get(); // 에셋 툴 모듈 로드
 *		
 *		EAssetTypeCategories::Type Category = AssetTools.RegisterAdvancedAssetCategory(FName("FC"),FText::FromString(TEXT("퐁크러시")));
 *		
 *		AssetTools.RegisterAssetTypeActions(MakeShared<FFCDataTypeActions>(Category, UFCExampleData::StaticClass()).ToSharedRef());
*/
class FCEXAMPLEEDITOR_API FFCExampleDataTypeActions : public FAssetTypeActions_Base
{
public:
	FFCExampleDataTypeActions(EAssetTypeCategories::Type InAssetCategory, UClass* InClassType)
	: AssetCategory(InAssetCategory), ClassType(InClassType) {};

	// IAssetTypeActions interface
	/** 표시할 이름 */
	virtual FText GetName() const override { return FText::FromString(TEXT("퐁크러시 테스트 오브젝트")); }
	/** 구분 색 */
	virtual FColor GetTypeColor() const override { return FColor(230, 205, 165); }
	/** 표시할 에셋 */
	virtual UClass* GetSupportedClass() const override { return ClassType; }
	/** 카테고리 분류 */
	virtual uint32 GetCategories() override { return AssetCategory; }
	
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;

	
	// ~ IAssetTypeActions interface

	
	void ExecuteReimport(TArray<TWeakObjectPtr<UFCExampleData>> Objects);

private:
	// 패키지 카테고리 정의
	EAssetTypeCategories::Type AssetCategory;
	UClass* ClassType;
};
