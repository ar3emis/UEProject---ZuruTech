// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZTPlayerController.h"
#include "GameFramework/Pawn.h"
#include "ZTPlayerPawn.generated.h"

DECLARE_DERIVED_EVENT(AZTPlayerPawn, FOnMeshDataSpawned, FOnMeshDataChildSpawn)

UCLASS()
class ZTPLAYERSYSTEM_API AZTPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AZTPlayerPawn(); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
