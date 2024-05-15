// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ScrollableAssetBox.h"

#define LOCTEXT_NAMESPACE "ScrollableAssetBoxWidget"

TSharedRef<SWidget> UScrollableAssetBox::RebuildWidget()
{
	ScrollableAssetBox = SNew(SScrollableAssetBox).InMeshDataAsset(MeshDataAsset).ThumbnailSizeScale(ThumbnailSizeScale);
	ScrollableAssetBox->OnAssetThumbnailSelected.BindUObject(this, &UScrollableAssetBox::HandleAssetThumbnailSelected);
	
	return ScrollableAssetBox.ToSharedRef();
}

void UScrollableAssetBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if(ScrollableAssetBox.IsValid())
	{
		ScrollableAssetBox->ThumbnailSizeScale = ThumbnailSizeScale;
		ScrollableAssetBox->RefreshAssetThumbnails();
	}
}

void UScrollableAssetBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	ScrollableAssetBox.Reset();
}

const FText UScrollableAssetBox::GetPaletteCategory()
{
	return LOCTEXT("PaletteCategory", "Panel");
}

void UScrollableAssetBox::HandleAssetThumbnailSelected(const FMeshData& MeshData)
{
	OnAssetThumbnailSelected.Broadcast(MeshData);
}

#undef LOCTEXT_NAMESPACE