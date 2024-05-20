// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGames/ZTPlayerSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ZTGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ZTPLAYERSYSTEM_API UZTGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	UZTPlayerSaveGame* GetCurrentSaveGame()
	{
		return CurrentSaveGame;
	}

	void CreateSaveGameSlot(const FString& SlotName);

	void SaveGame();
	void LoadGame(const FString& SlotName);

	FString GetCurrentSaveSlotName()
	{
		return CurrentSaveGameSlot;
	}
private:

	UPROPERTY()
	UZTPlayerSaveGame* CurrentSaveGame;
	
	UPROPERTY()
	FString CurrentSaveGameSlot;
	
};
