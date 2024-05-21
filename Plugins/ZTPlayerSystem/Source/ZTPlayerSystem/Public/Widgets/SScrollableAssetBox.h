// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/MeshDataAsset.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"

DECLARE_DELEGATE_OneParam(FAssetThumbnailSelected, const FMeshData&)


/**
 * 
 */
class ZTPLAYERSYSTEM_API SScrollableAssetBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScrollableAssetBox)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UMeshDataAsset>, InMeshDataAsset)
	SLATE_ARGUMENT(float, ThumbnailSizeScale)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FAssetThumbnailSelected OnAssetThumbnailSelected;

	float ThumbnailSizeScale;
	void RefreshAssetThumbnails();

private:

	
	TWeakObjectPtr<UMeshDataAsset> MeshDataAsset;
	TSharedPtr<SScrollBox> ScrollBox;


};
