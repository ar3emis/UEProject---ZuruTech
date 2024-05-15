// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SScrollableAssetBox.h"
#include "Components/Widget.h"
#include "ScrollableAssetBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAssetThumbnailSelectedEvent, const FMeshData&, MeshData);

/**
 * 
 */
UCLASS()
class ZTPLAYERSYSTEM_API UScrollableAssetBox : public UWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MeshData")
	UMeshDataAsset* MeshDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MeshData")
	float ThumbnailSizeScale = 1;;

	UPROPERTY(BlueprintAssignable, Category="Overlay Widget")
	FAssetThumbnailSelectedEvent OnAssetThumbnailSelected;

		
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual const FText GetPaletteCategory() override;
	void HandleAssetThumbnailSelected(const FMeshData& MeshData);
	
private:

	TSharedPtr<SScrollableAssetBox> ScrollableAssetBox;

	
};
