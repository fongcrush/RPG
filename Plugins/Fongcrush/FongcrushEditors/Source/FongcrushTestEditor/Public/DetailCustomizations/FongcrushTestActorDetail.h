// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class AFongcrushEditorTestActor;

class FONGCRUSHTESTEDITOR_API FFongcrushTestActorDetail : public IDetailCustomization
{
public:
	/**
	 * PropertyEditor 모듈의 커스텀 레이아웃 등록 메서드용 바인드 함수.
	 * 반환 타입 TSharedRef<IDetailCustomization> 델리게이트 매개변수 FOnGetDetailCustomizationInstance 에 바인드 하기 위한 인스턴스 Getter 함수다.
	 * */
	static TSharedRef<IDetailCustomization> MakeInstance() { return MakeShared<FFongcrushTestActorDetail>(); }
	/** 레이아웃 변경을 위한 재정의 함수 */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayoutBuilder) override;

protected:
	/** SCheckBox IsCheck 바인드  */
    ECheckBoxState IsModeRadioChecked(AFongcrushEditorTestActor* InActor, int OptionIndex) const;
	/** SCheckBox OnCheckStateChanged 바인드  */
	void OnModeRadioChanged(ECheckBoxState CheckType, AFongcrushEditorTestActor* InActor, int OptionIndex);
};
