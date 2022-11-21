#include "StableDiffusionToolsModule.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "StableDiffusionToolsSettings.h"
#include "StableDiffusionSubsystem.h"
#include "IPythonScriptPlugin.h"

#define LOCTEXT_NAMESPACE "FStableDiffusionToolsModule"

void FStableDiffusionToolsModule::StartupModule() 
{
	// Wait for Python to load our derived classes before we construct the settings object
	IPythonScriptPlugin& PythonModule = FModuleManager::LoadModuleChecked<IPythonScriptPlugin>(TEXT("PythonScriptPlugin"));
	PythonModule.OnPythonInitialized().AddRaw(this, &FStableDiffusionToolsModule::CreateSettingsSection);
}

void FStableDiffusionToolsModule::ShutdownModule() {

}

void FStableDiffusionToolsModule::CreateSettingsSection() {
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
			SettingsSection->OnModified().BindRaw(this, &FStableDiffusionToolsModule::HandleSettingsSaved);
		}
	}
}


bool FStableDiffusionToolsModule::HandleSettingsSaved() {
	UStableDiffusionToolsSettings* Settings = GetMutableDefault<UStableDiffusionToolsSettings>();
	if (Settings->GetGeneratorType()) {
		if (Settings->GetGeneratorType() != UStableDiffusionBridge::StaticClass()) {
			Settings->SaveConfig();
		}
	}

	UStableDiffusionSubsystem* SDSubSystem = GEditor->GetEditorSubsystem<UStableDiffusionSubsystem>();
	if (SDSubSystem) {
		SDSubSystem->CreateBridge();
		return true;
	}
	return false;
}


IMPLEMENT_MODULE(FStableDiffusionToolsModule, StableDiffusionTools)

#undef LOCTEXT_NAMESPACE
