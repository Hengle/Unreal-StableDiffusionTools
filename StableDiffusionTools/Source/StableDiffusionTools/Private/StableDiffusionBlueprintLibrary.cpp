// Fill out your copyright notice in the Description page of Project Settings.

#include "StableDiffusionBlueprintLibrary.h"
#include "SLevelViewport.h"

UStableDiffusionSubsystem* UStableDiffusionBlueprintLibrary::GetStableDiffusionSubsystem() {
	UStableDiffusionSubsystem* subsystem = nullptr;
	subsystem = GEditor->GetEditorSubsystem<UStableDiffusionSubsystem>();
	return subsystem;
}
