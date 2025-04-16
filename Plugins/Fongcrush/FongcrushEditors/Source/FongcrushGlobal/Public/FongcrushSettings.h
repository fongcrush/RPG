#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FongcrushSettings.generated.h"


UCLASS(Config = Editor, DefaultConfig)
class FONGCRUSHGLOBAL_API UFongcrushSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config, Category = "Spawner")
	float MaxDebugDrawDistance = 10000.f;

	static float GetMaxDebugDrawDistance(){ return GetDefault<UFongcrushSettings>()->MaxDebugDrawDistance; } 
};
