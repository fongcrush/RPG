#include "FongcrushGlobal.h"

#include "FongcrushSettings.h"
#include "ISettingsContainer.h"
#include "ISettingsModule.h"

DEFINE_LOG_CATEGORY(FongcrushGlobal)

#define LOCTEXT_NAMESPACE "FFongcrushGlobalModule"

void FFongcrushGlobalModule::StartupModule()
{
	UE_LOG(FongcrushGlobal, Display, TEXT("FongcrushGlobal 모듈 시작"))
	
	ISettingsModule& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	TSharedPtr<ISettingsContainer> ProjectSettingsContainer = SettingsModule.GetContainer("Project");
	
	ProjectSettingsContainer->DescribeCategory(
		TEXT("퐁크러시"),
		FText::FromString(TEXT("퐁크러시")),
		FText::FromString(TEXT("퐁크러시 카테고리")));

	SettingsModule.RegisterSettings("Project", TEXT("퐁크러시"), TEXT("일반"),
		FText::FromString(TEXT("일반")),
		FText::FromString(TEXT("퐁크러시 설정 모음")),
		GetMutableDefault<UFongcrushSettings>());
}

void FFongcrushGlobalModule::ShutdownModule()
{
	ISettingsModule& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	SettingsModule.UnregisterSettings("Project", TEXT("퐁크러시"), TEXT("일반"));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FFongcrushGlobalModule, FongcrushGlobal)