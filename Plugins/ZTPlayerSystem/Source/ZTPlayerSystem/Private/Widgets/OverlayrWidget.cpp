// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverlayrWidget.h"


void UOverlayrWidget::ReceiveMeshData(FMeshData MeshData)
{
	OnAssetThumbnailSelected.Broadcast(MeshData);
}

void UOverlayrWidget::InitializeWidget(UMeshDataAsset* MeshDataAsset)
{
	OnMeshDataInitialized(MeshDataAsset);
}

void UOverlayrWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
