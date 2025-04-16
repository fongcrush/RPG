

#pragma once

#include "Modules/ModuleManager.h"

class IModuleListener
{
public:
	virtual void OnStartupModule(){}
	virtual void OnShutdownModule(){}
};

class FONGCRUSHTESTEDITOR_API FFongcrushModuleBase : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
        if (!IsRunningCommandlet())
        {
            AddModuleListener();
            for (const TSharedRef<IModuleListener>& ModuleListener : ModuleListeners)
            {
                ModuleListener->OnStartupModule();
            }
        }
	};
	virtual void ShutdownModule() override
	{
        for (const TSharedRef<IModuleListener>& ModuleListener : ModuleListeners)
        {
            ModuleListener->OnShutdownModule();
        }
	};
	virtual void AddModuleListener() {};

protected:
	TArray<TSharedRef<IModuleListener>> ModuleListeners;
};