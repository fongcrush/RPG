#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FCSpawnerSettings.generated.h"


UCLASS(Config = Fongcrush, DefaultConfig, meta = (DisplayName = "Fongcrush"))
class FCSPAWNEREDITOR_API UFCSpawnerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UFCSpawnerSettings()
	{
		CategoryName = TEXT("Plugins");
		SectionName = TEXT("Fongcrush");
	};
	
	UPROPERTY(EditAnywhere, Config, Category = "Spawner")
	float MaxDebugDrawDistance = 3000.f;

	static float GetMaxDebugDrawDistance(){ return GetDefault<UFCSpawnerSettings>()->MaxDebugDrawDistance; } 
};
