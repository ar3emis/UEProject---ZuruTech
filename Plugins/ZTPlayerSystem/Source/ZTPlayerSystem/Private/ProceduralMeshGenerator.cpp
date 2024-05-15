// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshGenerator.h"


// Sets default values
AProceduralMeshGenerator::AProceduralMeshGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMesh;
}

// Called when the game starts or when spawned
void AProceduralMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralMeshGenerator::GenerateCube(FVector Dimensions)
{
    ProceduralMesh->ClearAllMeshSections();
    
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    // Each face of the cube needs four vertices to have proper UVs
    // Front face
    Vertices.Add(FVector(-Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2)); // 0
    Vertices.Add(FVector(Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2));  // 1
    Vertices.Add(FVector(-Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2));  // 2
    Vertices.Add(FVector(Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2));   // 3

    // Back face
    Vertices.Add(FVector(Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2)); // 4
    Vertices.Add(FVector(-Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2)); // 5
    Vertices.Add(FVector(Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2));  // 6
    Vertices.Add(FVector(-Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2)); // 7

    // Right face
    Vertices.Add(FVector(Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2));  // 8
    Vertices.Add(FVector(Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2)); // 9
    Vertices.Add(FVector(Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2));   // 10
    Vertices.Add(FVector(Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2));  // 11

    // Left face
    Vertices.Add(FVector(-Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2)); // 12
    Vertices.Add(FVector(-Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2));  // 13
    Vertices.Add(FVector(-Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2));  // 14
    Vertices.Add(FVector(-Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2));   // 15

    // Top face
    Vertices.Add(FVector(-Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2));  // 16
    Vertices.Add(FVector(Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2));   // 17
    Vertices.Add(FVector(-Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2)); // 18
    Vertices.Add(FVector(Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2));  // 19

    // Bottom face
    Vertices.Add(FVector(-Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2)); // 20
    Vertices.Add(FVector(Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2));  // 21
    Vertices.Add(FVector(-Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2));  // 22
    Vertices.Add(FVector(Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2));   // 23

    // Define triangles (2 per face, 6 faces)
    for (int32 i = 0; i < 24; i += 4)
    {
        Triangles.Append({i + 2, i + 1, i});
        Triangles.Append({i + 3, i + 1, i + 2});
    }

	// Initialize UVs array with enough space for all vertices.
	UVs.SetNum(24);

	// UVs for Front and Back faces (no change needed if these were correct)
	for (int32 i = 0; i < 8; i += 4) {
		UVs[i] = FVector2D(0.f, 0.f);
		UVs[i + 1] = FVector2D(1.f, 0.f);
		UVs[i + 2] = FVector2D(0.f, 1.f);
		UVs[i + 3] = FVector2D(1.f, 1.f);
	}

	// Correcting UVs for Right and Left faces by rotating them 90 degrees
	for (int32 i = 8; i < 16; i += 4) {
		UVs[i] = FVector2D(0.f, 1.f);
		UVs[i + 1] = FVector2D(0.f, 0.f);
		UVs[i + 2] = FVector2D(1.f, 1.f);
		UVs[i + 3] = FVector2D(1.f, 0.f);
	}

	// UVs for Top and Bottom faces (assuming these are correct, adjust if not)
	for (int32 i = 16; i < 24; i += 4) {
		UVs[i] = FVector2D(0.f, 0.f);
		UVs[i + 1] = FVector2D(1.f, 0.f);
		UVs[i + 2] = FVector2D(0.f, 1.f);
		UVs[i + 3] = FVector2D(1.f, 1.f);
	}

    // Initialize colors (white)
    Colors.Init(FLinearColor(1,1,1,1), Vertices.Num());


    // Compute normals and tangents
    Normals.Init(FVector(0, 0, 0), Vertices.Num());
    Tangents.Init(FProcMeshTangent(FVector(1, 0, 0), false), Vertices.Num());  // Initialize tangent array
    for (int i = 0; i < Triangles.Num(); i += 3)
    {
        int32 Index1 = Triangles[i];
        int32 Index2 = Triangles[i + 1];
        int32 Index3 = Triangles[i + 2];
        FVector Normal = FVector::CrossProduct(Vertices[Index2] - Vertices[Index1], Vertices[Index3] - Vertices[Index1]).GetSafeNormal();

        // Add normal to each vertex in the triangle
        Normals[Index1] += Normal;
        Normals[Index2] += Normal;
        Normals[Index3] += Normal;
    }
    for (FVector& Normal : Normals)
    {
        Normal.Normalize();
    }

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}


void AProceduralMeshGenerator::GenerateSphere(float Radius, int32 Segments, int32 Rings)
{
    ProceduralMesh->ClearAllMeshSections();

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

 
    for (int r = 0; r < Rings; ++r)
    {
        float phi = PI * r / (Rings + 1);
        for (int s = 0; s < Segments; ++s)
        {
            float theta = 2 * PI * s / Segments;
            FVector pos = FVector(Radius * FMath::Sin(phi) * FMath::Cos(theta), Radius * FMath::Sin(phi) * FMath::Sin(theta), Radius * FMath::Cos(phi));
            Vertices.Add(pos);
            Normals.Add(pos.GetSafeNormal());
            UVs.Add(FVector2D(s / (float)Segments, r / (float)(Rings + 1)));
            FVector tangent = FVector(-FMath::Sin(theta), FMath::Cos(theta), 0).GetSafeNormal();
            Tangents.Add(FProcMeshTangent(tangent, false));
            Colors.Add(FLinearColor(1, 1, 1, 1));

        	// We only add extra vertices at the seam on the first ring iteration
        	if (s == Segments) {
        		// Duplicate the first vertex of each ring at the seam for seamless UVs
        		Vertices.Add(Vertices[1 + (r-1)*(Segments+1)]);
        		Normals.Add(Normals[1 + (r-1)*(Segments+1)]);
        		UVs.Add(FVector2D(1.f, (float)r / (Rings + 1)));
        		Tangents.Add(Tangents[1 + (r-1)*(Segments+1)]);
        		Colors.Add(FLinearColor(1, 1, 1, 1));
        	}
        }
    }

    // Bottom vertex
    Vertices.Add(FVector(0, 0, -Radius));
    Normals.Add(FVector(0, 0, -1));
    UVs.Add(FVector2D(0.5f, 1));
    Tangents.Add(FProcMeshTangent(FVector(1, 0, 0), false));
    Colors.Add(FLinearColor(1, 1, 1, 1));

    // Top Cap
    for (int s = 0; s < Segments; ++s)
    {
        Triangles.Add(0);
        Triangles.Add(1 + s);
        Triangles.Add(1 + (s + 1) % Segments);
    }

    // Body
    for (int r = 0; r < Rings; ++r)
    {
        for (int s = 0; s < Segments; ++s)
        {
            int current = 1 + r * Segments + s;
            int next = current + 1;
            if (s == Segments - 1) next = 1 + r * Segments;  // Wrap around

            int below = current + Segments;
            int belowNext = next + Segments;

            Triangles.Add(current);
            Triangles.Add(next);
            Triangles.Add(belowNext);

            Triangles.Add(current);
            Triangles.Add(belowNext);
            Triangles.Add(below);
        }
    }

    // Bottom Cap
    int bottomIndex = Vertices.Num() - 1;
    int firstOfLastRing = 1 + (Rings - 1) * Segments;
    for (int s = 0; s < Segments; ++s)
    {
        Triangles.Add(bottomIndex);
        Triangles.Add(firstOfLastRing + (s + 1) % Segments);
        Triangles.Add(firstOfLastRing + s);
    }

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AProceduralMeshGenerator::GenerateCylinder(FVector Location, float Radius, float Height, int32 Segments)
{
    ProceduralMesh->ClearAllMeshSections();
    
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    // Generate the vertices, normals, UVs, and tangents for the cylinder
    for (int32 s = 0; s <= Segments; s++)
    {
        float Angle = static_cast<float>(s) / Segments * 2.0f * PI;
        float X = Radius * FMath::Cos(Angle);
        float Y = Radius * FMath::Sin(Angle);

        Vertices.Add(Location + FVector(X, Y, -Height / 2.0f));
        Vertices.Add(Location + FVector(X, Y, Height / 2.0f));

        Normals.Add(FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f));
        Normals.Add(FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f));

        UVs.Add(FVector2D(static_cast<float>(s) / Segments, 0.0f));
        UVs.Add(FVector2D(static_cast<float>(s) / Segments, 1.0f));

        FVector TangentX = FVector(-FMath::Sin(Angle), FMath::Cos(Angle), 0.0f);
        FVector TangentY = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);
        FVector TangentZ = FVector(0.0f, 0.0f, 1.0f);

        Tangents.Add(FProcMeshTangent(TangentX, false));
        Tangents.Add(FProcMeshTangent(TangentY, false));
    }

    // Generate the triangle indices for both sides
    for (int32 s = 0; s < Segments; s++)
    {
        int32 i0 = s * 2;
        int32 i1 = i0 + 1;
        int32 i2 = (s + 1) * 2;
        int32 i3 = i2 + 1;

        // Outer side
        Triangles.Add(i0);
        Triangles.Add(i2);
        Triangles.Add(i1);

        Triangles.Add(i1);
        Triangles.Add(i2);
        Triangles.Add(i3);

        // Inner side (reverse winding)
        Triangles.Add(i0);
        Triangles.Add(i1);
        Triangles.Add(i2);

        Triangles.Add(i1);
        Triangles.Add(i3);
        Triangles.Add(i2);
    }

    // Generate the vertex colors
    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // Add the mesh to the procedural mesh component
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AProceduralMeshGenerator::GenerateCone(FVector Location, float Radius, float Height, int32 Segments)
{
    ProceduralMesh->ClearAllMeshSections();
    
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    // Generate the vertices, normals, UVs, and tangents for the cone
    Vertices.Add(Location + FVector(0.0f, 0.0f, Height));
    Normals.Add(FVector(0.0f, 0.0f, 1.0f));
    UVs.Add(FVector2D(0.5f, 1.0f));
    Tangents.Add(FProcMeshTangent(FVector(1.0f, 0.0f, 0.0f), false));

    for (int32 s = 0; s <= Segments; s++)
    {
        float Angle = static_cast<float>(s) / Segments * 2.0f * PI;
        float X = Radius * FMath::Cos(Angle);
        float Y = Radius * FMath::Sin(Angle);

        Vertices.Add(Location + FVector(X, Y, 0.0f));
        Normals.Add(FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f));
        UVs.Add(FVector2D(static_cast<float>(s) / Segments, 0.0f));

        FVector TangentX = FVector(-FMath::Sin(Angle), FMath::Cos(Angle), 0.0f);
        FVector TangentY = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);
        FVector TangentZ = FVector(0.0f, 0.0f, 1.0f);

        Tangents.Add(FProcMeshTangent(TangentX, false));
    }

    // Generate the triangle indices
    for (int32 s = 0; s < Segments; s++)
    {
        Triangles.Add(0);
        Triangles.Add(s + 2);
        Triangles.Add(s + 1);
    }

    // Generate the vertex colors
    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // Add the mesh to the procedural mesh component
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AProceduralMeshGenerator::GenerateTube(FVector Location, float Radius, float Length, int32 Segments, FVector StartPoint, FVector EndPoint)
{
    ProceduralMesh->ClearAllMeshSections();
    
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    // Generate the vertices, normals, UVs, and tangents for the tube
    for (int32 s = 0; s <= Segments; s++)
    {
        float Angle = static_cast<float>(s) / Segments * 2.0f * PI;
        float X = Radius * FMath::Cos(Angle);
        float Y = Radius * FMath::Sin(Angle);

        FVector Position = FMath::Lerp(StartPoint, EndPoint, static_cast<float>(s) / Segments);
        FVector Normal = (EndPoint - StartPoint).GetSafeNormal();

        Vertices.Add(Location + FVector(X, Y, 0.0f) + Position);
        Normals.Add(Normal);
        UVs.Add(FVector2D(static_cast<float>(s) / Segments, 0.0f));

        FVector TangentX = FVector(-FMath::Sin(Angle), FMath::Cos(Angle), 0.0f);
        FVector TangentY = Normal ^ TangentX;
        FVector TangentZ = Normal;

        Tangents.Add(FProcMeshTangent(TangentX, false));
    }

    // Generate the triangle indices
    for (int32 s = 0; s < Segments; s++)
    {
        int32 i0 = s;
        int32 i1 = s + 1;

        Triangles.Add(i0);
        Triangles.Add(i1);
        Triangles.Add(i0 + Segments + 1);

        Triangles.Add(i1);
        Triangles.Add(i1 + Segments + 1);
        Triangles.Add(i0 + Segments + 1);
    }

    // Generate the vertex colors
    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // Add the mesh to the procedural mesh component
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}


void AProceduralMeshGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(MaterialInterface)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(ProceduralMesh->GetMaterial(0));
		if(!DynamicMaterial)
			 DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
		
		if(DynamicMaterial)
		{
			DynamicMaterial->SetScalarParameterValue("UniformTile", UniformTiling);
			DynamicMaterial->SetVectorParameterValue("BaseColor", BaseColor);
			ProceduralMesh->SetMaterial(0, DynamicMaterial);
		}
		
	}
	
	
}

