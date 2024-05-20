// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ZTPlayerSaveGame.generated.h"

USTRUCT()
struct FSaveSlotData
{
	GENERATED_BODY()

	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	UStaticMesh* ActorMesh;

	UPROPERTY()
	FTransform ActorTransform;
	
};

USTRUCT()
struct FZTSaveSlot
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FSaveSlotData> SavedActors;
	
};

/**
 * 
 */
UCLASS()
class ZTPLAYERSYSTEM_API UZTPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FString, FZTSaveSlot> SaveSlots;

	
};
