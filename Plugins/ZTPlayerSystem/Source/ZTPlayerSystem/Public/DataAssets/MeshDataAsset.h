// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"
USTRUCT(Blueprintable)
struct FMeshData
{
    GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh")
	FString MeshName;
	
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh")
    UStaticMesh* Mesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh")
	UTexture* Thumbnail;
};
/**
 * 
 */
UCLASS(BlueprintType)
class ZTPLAYERSYSTEM_API UMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TArray<FMeshData> MeshData;
};
