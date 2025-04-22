#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(FCSpawnerEditor, Log, All)

class FFCSpawnerEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
