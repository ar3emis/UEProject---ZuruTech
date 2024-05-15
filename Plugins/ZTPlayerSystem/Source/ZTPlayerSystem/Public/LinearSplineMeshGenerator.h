// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "LinearSplineMeshGenerator.generated.h"

UCLASS()
class ZTPLAYERSYSTEM_API ALinearSplineMeshGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALinearSplineMeshGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Defaults)
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Defaults)
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Defaults)
	UMaterialInterface* BaseMaterial;

	void ConstructMesh();
};
