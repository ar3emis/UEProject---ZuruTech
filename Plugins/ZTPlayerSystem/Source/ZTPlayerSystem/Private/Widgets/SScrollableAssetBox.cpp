// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SScrollableAssetBox.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SScrollableAssetBox::Construct(const FArguments& InArgs)
{
	ScrollBox = SNew(SScrollBox);
	MeshDataAsset = InArgs._InMeshDataAsset;
	ThumbnailSizeScale = InArgs._ThumbnailSizeScale;
	ChildSlot
	[
	  ScrollBox.ToSharedRef()
		
	];

	ScrollBox->SetOrientation(Orient_Horizontal);
	RefreshAssetThumbnails();
}

void SScrollableAssetBox::RefreshAssetThumbnails()
{
	ScrollBox->ClearChildren();
	if(MeshDataAsset.IsValid())
	{
		for(const FMeshData& MeshData : MeshDataAsset->MeshData)
		{
			if(UTexture2D* ThumbnailTexture = Cast<UTexture2D>(MeshData.Thumbnail))
			{
				FSlateBrush* ThumbnailBrush = new FSlateBrush();
				ThumbnailBrush->SetResourceObject(ThumbnailTexture);
				ThumbnailBrush->ImageSize = FVector2D(ThumbnailTexture->GetSizeX() * ThumbnailSizeScale, ThumbnailTexture->GetSizeY());

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand)
				.OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
				{
					if(MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
					{
						OnAssetThumbnailSelected.ExecuteIfBound(MeshData);
						return FReply::Handled();
					}

					return FReply::Unhandled();
				});

				ScrollBox->AddSlot()
				[
					ThumbnailImage.ToSharedRef()
				];
			}
		}
	}
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
