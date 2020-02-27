// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Modules/ModuleInterface.h"
#include "ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "UnrealEd.h"
#include "ConversationAssetActions.h"

class FTopDownEditorEditorModule : public IModuleInterface
{
	void CommandDoSpawn(const TArray<FString>& params, UWorld* world);
    void StartupModule() override;
    void ShutdownModule() override;

protected:
	IConsoleCommand* registeredCommand = nullptr;
	TSharedPtr<FConversationAssetActions> conversationActions;
};