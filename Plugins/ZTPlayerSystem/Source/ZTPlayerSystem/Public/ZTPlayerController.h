// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/MeshDataAsset.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/PlayerController.h"
#include "SaveGames/ZTPlayerSaveGame.h"
#include "Widgets/OverlayrWidget.h"
#include "ZTPlayerController.generated.h"


DECLARE_EVENT_OneParam(AZTPlayerController, FOnMeshDataSpawned, FMeshData)


// DECLARE_MULTICAST_DELEGATE_OneParam(FOnMeshDataSpawned, FMeshData)


UCLASS()
class ZTPLAYERSYSTEM_API AZTPlayerController : public APlayerController
{
	GENERATED_BODY()

	
protected:

	AZTPlayerController();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MeshData")
	UMeshDataAsset* MeshDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UOverlayrWidget> OverlayWidgetClass;

	UPROPERTY()
	UOverlayrWidget* OverlayWidget;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Trace")
	void OnFloorDetected();

	
	UPROPERTY()
	TArray<FSaveSlotData> SaveSlotMeshInfo;
	

	virtual void BeginPlay() override;
	void HandleMeshDataSpawned(FMeshData MeshData);
	void HandleMeshDataChildSpawned(FMeshData MeshData);
	virtual void SetupInputComponent() override;


	void ProcessMouseClick();


	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void LoadSlot();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MeshData")
	UMaterialInterface* BaseMaterial;

	UPROPERTY()
	FString SaveSlotName;
	
	UPROPERTY()
	FVector LastHitLocation;

	UFUNCTION()
	void SpawnMeshFromMeshData(const FMeshData& MeshData);


	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void SaveSlot();

	FOnMeshDataSpawned OnMeshDataSpawned;
	FOnMeshDataSpawned OnMeshDataChildSpawn;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSomeFunctions(float P);
};
