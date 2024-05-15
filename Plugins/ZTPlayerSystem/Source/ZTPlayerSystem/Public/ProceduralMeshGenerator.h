// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshGenerator.generated.h"

UCLASS()
class ZTPLAYERSYSTEM_API AProceduralMeshGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProceduralMeshGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Defaults)
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Defaults)
	UMaterialInterface* MaterialInterface;

	UFUNCTION(BlueprintCallable, Category=Defauls)
	void GenerateCube(FVector Dimensions);

	UFUNCTION(BlueprintCallable, Category=Defauls)
	void GenerateSphere(float Radius, int32 Segments, int32 Rings);

	UFUNCTION(BlueprintCallable, Category=Defauls)
	void GenerateCylinder(FVector Location, float Radius, float Height, int32 Segments);

	UFUNCTION(BlueprintCallable, Category=Defauls)
	void GenerateCone(FVector Location, float Radius, float Height, int32 Segments);

	UFUNCTION(BlueprintCallable, Category=Defauls)
	void GenerateTube(FVector Location, float Radius, float Length, int32 Segments, FVector StartPoint,
	                  FVector EndPoint);

	virtual void OnConstruction(const FTransform& Transform) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Defaults)
	float UniformTiling = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Defaults)
	FLinearColor BaseColor = FLinearColor(1,1,1,1);
	
	
};
