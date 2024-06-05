// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsyncScatterTask.h"
#include "ScatterDataAsset.h"
#include "Components/BoxComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ScatterActor.generated.h"

UCLASS()
class PROCEDURALOBJECTSCATTER_API AScatterActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AScatterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category="Scatter")
	TSoftObjectPtr<UScatterDataAsset> DataAsset;
	
	
	UPROPERTY(EditAnywhere, Category="Scatter")
	int32 NumberOfInstances = 100;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Defaults)
	UBoxComponent* BoundingVolume;

	UFUNCTION(BlueprintCallable)
	void ScatterObjects();

	void AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms);
	void FinishScatter();
private:

	FAsyncTask<FAsyncScatterTask>* AsyncScatterTask;

	UPROPERTY()
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;

	FScopedSlowTask* SlowTask;
};
