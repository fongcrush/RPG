// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(FCSpawner, Log, All);

class FFCSpawnerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FDelegateHandle ReInstancedHandle;
	FDelegateHandle PreChangedHandle;
	FDelegateHandle PostChangedHandle;
};



inline FString GetWorldTypeString(UObject* const& Object)
{
	if (!Object)
	{
		return "Invalid Object";
	}

	if (UWorld* World = Object->GetWorld())
	{
		FString WorldMemoryPath = FString::Printf(TEXT("%p"), World);
		switch (World->WorldType)
		{
		case EWorldType::None: return "None World(" + WorldMemoryPath + ")";
		case EWorldType::Game: return "Game World(" + WorldMemoryPath + ")";
		case EWorldType::Editor: return "Editor World(" + WorldMemoryPath + ")";
		case EWorldType::PIE: return "PIE World(" + WorldMemoryPath + ")";
		case EWorldType::EditorPreview: return "EditorPreview World(" + WorldMemoryPath + ")";
		case EWorldType::GamePreview: return "GamePreview World(" + WorldMemoryPath + ")";
		case EWorldType::GameRPC: return "GameRPC World(" + WorldMemoryPath + ")";
		case EWorldType::Inactive: return "Inactive World(" + WorldMemoryPath + ")";
		default: break;
		}
	}
	return "InValid World";
}