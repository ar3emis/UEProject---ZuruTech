// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScatterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALOBJECTSCATTER_API UScatterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category="Scatter")
	TArray<UStaticMesh*> StaticMeshes;
};
