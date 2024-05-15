// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/MeshDataAsset.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/OverlayrWidget.h"
#include "ZTPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class ZTPLAYERSYSTEM_API AZTPlayerController : public APlayerController
{
	GENERATED_BODY()

	
protected:

	AZTPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MeshData")
	UMaterialInterface* BaseMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MeshData")
	UMeshDataAsset* MeshDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UOverlayrWidget> OverlayWidgetClass;

	UPROPERTY()
	UOverlayrWidget* OverlayWidget;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Trace")
	void OnFloorDetected();

	UPROPERTY()
	FVector LastHitLocation;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void SpawnMeshFromMeshData(const FMeshData& MeshData);

	void ProcessMouseClick();
};
