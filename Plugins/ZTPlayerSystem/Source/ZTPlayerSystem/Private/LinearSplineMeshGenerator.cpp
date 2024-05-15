// Fill out your copyright notice in the Description page of Project Settings.


#include "LinearSplineMeshGenerator.h"


// Sets default values
ALinearSplineMeshGenerator::ALinearSplineMeshGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMesh;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALinearSplineMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALinearSplineMeshGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	for(int iIndex = 0; iIndex < SplineComponent->GetNumberOfSplinePoints(); iIndex++)
	{
		FVector Tangent = FVector::ZeroVector;
		SplineComponent->SetTangentAtSplinePoint(iIndex, Tangent, ESplineCoordinateSpace::Local);
	}

	ConstructMesh();
}

// Called every frame
void ALinearSplineMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALinearSplineMeshGenerator::ConstructMesh()
{
	ProceduralMesh->ClearAllMeshSections();
	
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	int32 NumberofPoints = SplineComponent->GetNumberOfSplinePoints();
	if(NumberofPoints < 3)
	{
		return;
	}

	FVector MinBounds = FVector(FLT_MAX);
	FVector MaxBounds = FVector(-FLT_MAX);

	for(int iIndex = 0; iIndex < NumberofPoints; iIndex++)
	{
		FVector P = SplineComponent->GetLocationAtSplinePoint(iIndex , ESplineCoordinateSpace::Local);
		MinBounds = MinBounds.ComponentMin(P);
		MaxBounds = MaxBounds.ComponentMax(P);
	}

	FVector BoundSize = MaxBounds - MinBounds;

	for(int iIndex = 0; iIndex < NumberofPoints; iIndex++)
	{
		int32 NextIndex = (iIndex + 1) % NumberofPoints; 
		FVector P = SplineComponent->GetLocationAtSplinePoint(iIndex , ESplineCoordinateSpace::Local);
		FVector NextP = SplineComponent->GetLocationAtSplinePoint(NextIndex , ESplineCoordinateSpace::Local);

		Vertices.Add(P);

		Normals.Add(FVector::UpVector);

		FVector LocalPos = P - MinBounds;
		FVector2D BoxUV(LocalPos.X / BoundSize.X, LocalPos.Y / BoundSize.Y);
		UVs.Add(BoxUV);

		FVector Tangent = (NextP - P).GetSafeNormal();
		Tangents.Add(FProcMeshTangent(Tangent, false));

		Colors.Add(FLinearColor(1,1,1,1));
	}

	for(int iIndex = 0; iIndex < NumberofPoints - 2; iIndex++)
	{
		FVector P1 = Vertices[iIndex];
		FVector P2 = Vertices[iIndex + 1];
		FVector P3 = Vertices[iIndex + 2];

		FVector Normal = FVector::CrossProduct(P2 - P1 , P3 - P1).GetSafeNormal();

		if(FVector::DotProduct(Normal, FVector::UpVector) < 0)
		{
		    Triangles.Add(iIndex + 1);
			Triangles.Add(iIndex + 2);
			Triangles.Add(iIndex);
		}
		else
		{
			Triangles.Add(iIndex + 2);
			Triangles.Add(iIndex + 1);
			Triangles.Add(iIndex);
		}
	
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);

	if(BaseMaterial)
	{
		ProceduralMesh->SetMaterial(0, BaseMaterial);
	}
	
}

