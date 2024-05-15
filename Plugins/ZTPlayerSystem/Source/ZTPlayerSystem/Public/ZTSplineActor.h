// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ZTSplineActor.generated.h"

UENUM(BlueprintType)
enum class EMeshType : uint8
{
	SplineMesh,
	StaticMesh
};

UCLASS()
class ZTPLAYERSYSTEM_API AZTSplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZTSplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Defaults)
	USceneComponent* RootScene;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Defaults)
	USplineComponent* SourceSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMesh* SourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TEnumAsByte<EMeshType> MeshType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	float DistanceBetweenMeshes = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TEnumAsByte<ESplineMeshAxis::Type> MeshForwardAxis = ESplineMeshAxis::X;

	UFUNCTION(BlueprintCallable, Category="Spline")
	void GenerateSplineMeshes();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Spline")
	TArray<USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Spline")
	TArray<UStaticMeshComponent*> StaticMeshComponents;

	void GenerateSplineMeshComponents();
	void GenerateStaticMeshComponents();
};
