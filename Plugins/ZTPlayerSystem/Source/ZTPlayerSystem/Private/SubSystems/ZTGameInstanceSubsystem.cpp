// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/ZTGameInstanceSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UZTGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UZTGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UZTGameInstanceSubsystem::CreateSaveGameSlot(const FString& SlotName)
{
	if(!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<UZTPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UZTPlayerSaveGame::StaticClass()));
	}
	
	CurrentSaveGame->SaveSlots.Add(SlotName, FZTSaveSlot());
	CurrentSaveGameSlot = SlotName;
	
}

void UZTGameInstanceSubsystem::SaveGame()
{
	if(CurrentSaveGame && !CurrentSaveGameSlot.IsEmpty())
	{
		UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSaveGameSlot, 0);
	}
}

void UZTGameInstanceSubsystem::LoadGame(const FString& SlotName)
{
	if(UZTPlayerSaveGame* LoadedSaveGame = Cast<UZTPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
	{
		CurrentSaveGame = LoadedSaveGame;
	}
	else
	{
		CurrentSaveGame = Cast<UZTPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UZTPlayerSaveGame::StaticClass()));
	}

	if(CurrentSaveGame->SaveSlots.Contains(SlotName))
	{
		CurrentSaveGameSlot = SlotName;
	}
	
}
