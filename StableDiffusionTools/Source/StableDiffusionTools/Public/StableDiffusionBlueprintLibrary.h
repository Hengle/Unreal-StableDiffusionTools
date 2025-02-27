// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StableDiffusionSubsystem.h"
#include "DependencySettings.h"
#include "Engine/SceneCapture.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/GeometryScriptSelectionTypes.h"
#include "UDynamicMesh.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Layers/LayersSubsystem.h"
#include "ProjectionBakeSession.h"
#include "StableDiffusionToolsSettings.h"
#include "StableDiffusionBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class STABLEDIFFUSIONTOOLS_API UStableDiffusionBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "StableDiffusion|Subsystem")
	static UStableDiffusionSubsystem* GetStableDiffusionSubsystem();

	UFUNCTION(BlueprintCallable, Category = "StableDiffusion|Subsystem")
	static ULayersSubsystem* GetLayersSubsystem();

	UFUNCTION(BlueprintCallable, Category = "StableDiffusion|Options")
	static UStableDiffusionToolsSettings* GetPluginOptions();

	UFUNCTION(BlueprintCallable, Category = "StableDiffusion|Options")
	static UDependencySettings* GetDependencyOptions();

	UFUNCTION(BlueprintCallable, Category = "Editor")
	static void RestartEditor();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FVector2D ProjectSceneCaptureWorldToUV(const FVector& WorldPosition, USceneCaptureComponent2D* SceneCapture, bool& BehindCamera);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FVector2D ProjectViewportWorldToUV(const FVector& WorldPosition, bool& BehindCamera);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FTransform GetEditorViewportCameraTransform();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FMatrix GetEditorViewportViewProjectionMatrix();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FMatrix GetEditorViewportViewMatrix();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FMinimalViewInfo GetEditorViewportViewInfo();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FIntPoint GetEditorViewportSize();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FVector GetEditorViewportDirection();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static TArray<AActor*> GetActorsInViewFrustum(const UObject* WorldContextObject, const FMatrix& ViewProjectionMatrix, const FVector& CameraLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Texture")
	static void CopyTextureDataUsingUVs(UTexture2D* SourceTexture, UTexture2D* TargetTexture, const FIntPoint& ScreenSize, const FMatrix& ViewProjectionMatrix, UDynamicMesh* SourceMesh, const TArray<int> TriangleIDs, bool ClearCoverageMask);

	UFUNCTION(BlueprintCallable, Category = "Texture")
	static FColor GetUVPixelFromTexture(UTexture2D* Texture, FVector2D UV);

	UFUNCTION(BlueprintCallable, Category = "Texture")
	static UTexture2D* ColorBufferToTexture(const TArray<FColor>& FrameColors, const FIntPoint& FrameSize, UTexture2D* OutTexture);

	UFUNCTION(BlueprintCallable, Category = "Texture")
	static UTexture2D* CreateTextureAsset(const FString& AssetPath, const FString& Name, FIntPoint Size, ETextureSourceFormat Format = ETextureSourceFormat::TSF_BGRA8, FColor Fill = FColor::Black);

	UFUNCTION(BlueprintCallable, Category = "Texture")
	static UProjectionBakeSessionAsset* CreateProjectionBakeSessionAsset(const FProjectionBakeSession& Session, const FString& AssetPath, const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Texture")
	static FColor LerpColor(const FColor& ColorA, const FColor& ColorB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Texture")
	static UMaterialInstanceConstant* CreateMaterialInstanceAsset(UMaterial* ParentMaterial, const FString& Path, const FString& Name);

	static UTexture2D* ColorBufferToTexture(const uint8* FrameData, const FIntPoint& FrameSize, UTexture2D* OutTex);
};
