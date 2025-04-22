// Copyright Epic Games, Inc. All Rights Reserved.

#include "FCSpawner.h"

DEFINE_LOG_CATEGORY(FCSpawner)

#define LOCTEXT_NAMESPACE "FFCSpawnerModule"

void FFCSpawnerModule::StartupModule()
{
	UE_LOG(FCSpawner, Display, TEXT("FCSpawner 모듈 시작"))
}

void FFCSpawnerModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFCSpawnerModule, FCSpawner)