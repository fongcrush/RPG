#pragma once

#include "CoreMinimal.h"

class FFCSpawnerEdModeModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
