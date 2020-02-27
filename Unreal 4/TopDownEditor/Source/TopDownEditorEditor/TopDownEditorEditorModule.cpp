#include "TopDownEditorEditorModule.h"
#include "SpawnerActor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"



IMPLEMENT_MODULE(FTopDownEditorEditorModule, TopDownEditorEditor)

void FTopDownEditorEditorModule::StartupModule()
{
	IConsoleManager& console = IConsoleManager::Get();
	registeredCommand = console.RegisterConsoleCommand
	(
		TEXT("DoSpawn"),TEXT("Need 1 parameter, will call the SpawnActor method of the given ASpawnerActor name"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(this, &FTopDownEditorEditorModule::CommandDoSpawn)
	);

	FAssetToolsModule& toolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& assetToolsModule = toolsModule.Get();
	conversationActions = MakeShareable(new FConversationAssetActions());
	//RegisterAssetTypeActions needs a shared ref, but we need to initialise to nullptr
	//so we need SharedPtr not SharedRef, we need to change it to a sharedRef
	assetToolsModule.RegisterAssetTypeActions(conversationActions.ToSharedRef());
}
void FTopDownEditorEditorModule::ShutdownModule()
{
	if (registeredCommand != nullptr)
	{
		IConsoleManager::Get().UnregisterConsoleObject(registeredCommand, false);
	}
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule& toolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
		IAssetTools& assetToolsModule = toolsModule.Get();
		assetToolsModule.UnregisterAssetTypeActions(conversationActions.ToSharedRef());
	}
	conversationActions.Reset();
}

void FTopDownEditorEditorModule::CommandDoSpawn(const TArray<FString>& params, UWorld* world)
{
	if (params.Num() <= 0)
	{
		return;
	}

	const FString& actorName = params[0];
	for (TActorIterator<ASpawnerActor> actIt(world); actIt; ++actIt)
	{
		if (actIt->GetName().Equals(actorName))
		{
			actIt->SpawnActor();
		}
	}
}