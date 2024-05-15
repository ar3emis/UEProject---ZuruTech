// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DelegateExamples.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "FlycamPawn.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnMovementDelegate, FVector, Value);

UCLASS()
class ZTPLAYERSYSTEM_API AFlycamPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFlycamPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly, Category=Defaults)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleDefaultsOnly, Category=Defaults)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, Category=Defaults)
	UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, Category=Defaults)
	UFloatingPawnMovement* FloatingPawnMovement;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPawnMoved(FVector MovementVector);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UPROPERTY()
	UDelegateExamples* DelegateExamples;

	UPROPERTY(BlueprintAssignable)
	FPawnMovementDelegate PawnMovementDelegate;

	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleZTDynamicDelegate(FVector Value);
};
