// Copyright Epic Games, Inc. All Rights Reserved.

#include "StableDiffusionToolsEditor.h"
#include "StableDiffusionToolsStyle.h"
#include "StableDiffusionToolsCommands.h"
#include "StableDiffusionToolsSettings.h"
#include "SDDependencyInstallerWidget.h"
#include "StableDiffusionSubsystem.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Engine/ObjectLibrary.h"
#include "AssetRegistryModule.h"
#include "EditorUtilityWidget.h"
#include "EditorAssetLibrary.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "EditorUtilitySubsystem.h"

static const FName StableDiffusionToolsTabName("Stable Diffusion Tools");
static const FName StableDiffusionDependencyInstallerTabName("Stable Diffusion Dependency Installer");

#define LOCTEXT_NAMESPACE "FStableDiffusionToolsEditorModule"

void FStableDiffusionToolsEditorModule::StartupModule()
{
	FStableDiffusionToolsStyle::Initialize();
	FStableDiffusionToolsStyle::ReloadTextures();
	FStableDiffusionToolsCommands::Register();
	
	// Register commands
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FStableDiffusionToolsCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FStableDiffusionToolsEditorModule::PluginButtonClicked),
		FCanExecuteAction());
	PluginCommands->MapAction(
		FStableDiffusionToolsCommands::Get().OpenDependenciesWindow,
		FExecuteAction::CreateRaw(this, &FStableDiffusionToolsEditorModule::OpenDependencyInstallerWindow),
		FCanExecuteAction());

	// Create menus
	{
		TSharedPtr<FExtender> NewMenuExtender = MakeShareable(new FExtender);
		NewMenuExtender->AddMenuExtension("LevelEditor",
			EExtensionHook::After,
			PluginCommands,
			FMenuExtensionDelegate::CreateRaw(this, &FStableDiffusionToolsEditorModule::AddMenuEntry));

		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(NewMenuExtender);
	}

	// Create settings
#if WITH_EDITOR
// register settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "StableDiffusionTools",
			LOCTEXT("StableDiffusionToolsSettingsName", "Stable Diffusion Tools"),
			LOCTEXT("StableDiffusionToolsSettingsDescription", "Configure the Stable Diffusion tools plug-in."),
			GetMutableDefault<UStableDiffusionToolsSettings>()
		);

		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FStableDiffusionToolsEditorModule::HandleSettingsSaved);
		}
	}
#endif // WITH_EDITOR
}

bool FStableDiffusionToolsEditorModule::HandleSettingsSaved() {
	UStableDiffusionSubsystem* SDSubSystem = GEditor->GetEditorSubsystem<UStableDiffusionSubsystem>();
	if (SDSubSystem) {
		SDSubSystem->CreateBridge();
		return true;
	}
	return false;
}

void FStableDiffusionToolsEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FStableDiffusionToolsStyle::Shutdown();
	FStableDiffusionToolsCommands::Unregister();
}

void FStableDiffusionToolsEditorModule::PluginButtonClicked()
{
	FString DependencyBPPath = "/StableDiffusionTools/UI/Widgets/BP_StableDiffusionViewportWidget.BP_StableDiffusionViewportWidget";
	CreatePanel(DependencyBPPath, "SD Tools");
}

void FStableDiffusionToolsEditorModule::OpenDependencyInstallerWindow() 
{
	FString DependencyBPPath = "/StableDiffusionTools/UI/Widgets/BP_StableDiffusionDependencyInstallerWidget.BP_StableDiffusionDependencyInstallerWidget";
	CreatePanel(DependencyBPPath, "SD Dependencies");
}

void FStableDiffusionToolsEditorModule::CreatePanel(const FString& BlueprintAssetPath, const FString& PanelLabel)
{
	UEditorUtilitySubsystem* EUSubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	UEditorUtilityWidgetBlueprint* EditorWidget = LoadObject<UEditorUtilityWidgetBlueprint>(NULL, *BlueprintAssetPath, NULL, LOAD_Verify, NULL);
	EUSubsystem->SpawnAndRegisterTab(EditorWidget);

	// Find created tab so we can rename it
	FName TabID;
	EUSubsystem->SpawnAndRegisterTabAndGetID(EditorWidget, TabID);
	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
	if (auto Tab = LevelEditorTabManager->FindExistingLiveTab(TabID)) {
		Tab->SetLabel(FText::FromString(PanelLabel));
	}
}

void FStableDiffusionToolsEditorModule::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("StableDiffusionToolsMenu", TAttribute<FText>(FText::FromString("Stable Diffusion Tools")));
	MenuBuilder.AddMenuEntry(FStableDiffusionToolsCommands::Get().OpenPluginWindow);
	MenuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStableDiffusionToolsEditorModule, StableDiffusionToolsEditor)