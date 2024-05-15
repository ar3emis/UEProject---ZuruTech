// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScrollableAssetBox.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/MeshDataAsset.h"
#include "OverlayrWidget.generated.h"


/**
 * 
 */
UCLASS()
class ZTPLAYERSYSTEM_API UOverlayrWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category="Overlay Widget")
	void OnMeshDataInitialized(UMeshDataAsset* MeshDataAsset);
	
	UPROPERTY(BlueprintAssignable, Category="Overlay Widget")
	FAssetThumbnailSelectedEvent OnAssetThumbnailSelected;

	UFUNCTION(BlueprintCallable, Category="Overlay Widget")
	void ReceiveMeshData(FMeshData MeshData);
	
	void InitializeWidget(UMeshDataAsset* MeshDataAsset);

	virtual void NativeConstruct() override;
	
};
