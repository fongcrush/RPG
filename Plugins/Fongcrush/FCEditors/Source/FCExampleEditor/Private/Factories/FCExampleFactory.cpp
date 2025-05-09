﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/FCExampleFactory.h"

#include "Objects/FCExampleData.h"


UFCExampleFactory::UFCExampleFactory()
{	
	SupportedClass = UFCExampleData::StaticClass(); // 지원(허용)할 베이스 클래스
	bCreateNew = true; // true면 엔진 자체 생성 전용이라 외부 파일 Import 불가능,  false면 외부 파일 Import 전용(에디터에서 생성 불가)
	bEditAfterNew = false; // 새로 만든 후 에디터 열기 여부
}

UObject* UFCExampleFactory::FactoryCreateNew(
	UClass* Class,
	UObject* InParent,
	FName Name,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn)
{
	return NewObject<UFCExampleData>(InParent, Class, Name, Flags | RF_Transactional);
}