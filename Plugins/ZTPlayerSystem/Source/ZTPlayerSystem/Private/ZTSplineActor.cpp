// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTSplineActor.h"

// Sets default values
AZTSplineActor::AZTSplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>("RootScene");
	RootComponent = RootScene;
	
	SourceSpline = CreateDefaultSubobject<USplineComponent>("SourceSpline");
	SourceSpline->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	

}

// Called when the game starts or when spawned
void AZTSplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZTSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZTSplineActor::GenerateSplineMeshes()
{
	for (int iIndex = 0; iIndex < SplineMeshComponents.Num(); iIndex++)
	{

		if(SplineMeshComponents[iIndex])
		{
			SplineMeshComponents[iIndex]->DestroyComponent();
			SplineMeshComponents[iIndex] = nullptr;
		}
	}

	SplineMeshComponents.Empty();

	for (int iIndex = 0; iIndex < StaticMeshComponents.Num(); iIndex++)
	{

		if(StaticMeshComponents[iIndex])
		{
			StaticMeshComponents[iIndex]->DestroyComponent();
			StaticMeshComponents[iIndex] = nullptr;
		}
	}

	StaticMeshComponents.Empty();


	switch (MeshType)
	{
	case EMeshType::SplineMesh:
		GenerateSplineMeshComponents();
		break;
	case EMeshType::StaticMesh:
		GenerateStaticMeshComponents();
		break;
	}
	
	
}

void AZTSplineActor::GenerateSplineMeshComponents()
{
	int32 NumberOfPoints = SourceSpline->GetNumberOfSplinePoints();

	for (int iIndex = 0; iIndex < NumberOfPoints; iIndex++)
	{
		if(iIndex < NumberOfPoints - 1)
		{
			
			FVector StartPosition, StartTangent;
			SourceSpline->GetLocationAndTangentAtSplinePoint(iIndex, StartPosition, StartTangent, ESplineCoordinateSpace::Local);

			FVector EndPosition, EndTangent;
			SourceSpline->GetLocationAndTangentAtSplinePoint(iIndex + 1, EndPosition, EndTangent, ESplineCoordinateSpace::Local);

			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			SplineMesh->RegisterComponent();
			SplineMesh->SetStaticMesh(SourceMesh);
			SplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
			SplineMesh->AttachToComponent(SourceSpline, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->ForwardAxis = MeshForwardAxis;

			SplineMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);

			SplineMeshComponents.Add(SplineMesh);
		}
		
	}
}

void AZTSplineActor::GenerateStaticMeshComponents()
{
	float SplineLength = SourceSpline->GetSplineLength();
	for (int iIndex = 0; iIndex < SplineLength; iIndex+=DistanceBetweenMeshes)
	{
		FVector Position = SourceSpline->GetLocationAtDistanceAlongSpline(iIndex, ESplineCoordinateSpace::Local);
		FRotator Rotation = SourceSpline->GetRotationAtDistanceAlongSpline(iIndex, ESplineCoordinateSpace::Local);
		
		UStaticMeshComponent* StaticMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
		StaticMesh->RegisterComponent();
		StaticMesh->SetStaticMesh(SourceMesh);
		StaticMesh->AttachToComponent(SourceSpline, FAttachmentTransformRules::KeepRelativeTransform);

		StaticMesh->SetRelativeLocationAndRotation(Position, Rotation);
		StaticMeshComponents.Add(StaticMesh);
	}
}


