// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"

AZTPlayerController::AZTPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AZTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(OverlayWidgetClass)
	{
		OverlayWidget = CreateWidget<UOverlayrWidget>(this, OverlayWidgetClass);
		if(OverlayWidget)
		{
			OverlayWidget->OnAssetThumbnailSelected.AddDynamic(this, &AZTPlayerController::SpawnMeshFromMeshData);
			
		}
	}
}

void AZTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

		UInputAction* ClickAction = NewObject<UInputAction>();
		ClickAction->ValueType = EInputActionValueType::Boolean;

		InputMappingContext->MapKey(ClickAction, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AZTPlayerController::ProcessMouseClick);

		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AZTPlayerController::SpawnMeshFromMeshData(const FMeshData& MeshData)
{
	if(MeshData.Mesh)
	{
		FBox BoundingBox = MeshData.Mesh->GetBoundingBox();
		FVector MinBounds = BoundingBox.Min;
		FVector MaxBounds = BoundingBox.Max;

		float OffsetZ = -MinBounds.Z;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if(AStaticMeshActor* SpawnedMesh = GetWorld()->SpawnActor<AStaticMeshActor>(LastHitLocation + FVector(0, 0, OffsetZ), FRotator::ZeroRotator, SpawnParameters))
		{
			SpawnedMesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			SpawnedMesh->GetStaticMeshComponent()->SetStaticMesh(MeshData.Mesh);

			if(BaseMaterial)
			{
				if(UMaterialInstanceDynamic* DynamicMatInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this))
				{
					DynamicMatInstance->SetScalarParameterValue("Tiling", 5);
					DynamicMatInstance->SetVectorParameterValue("BaseColor", FLinearColor::Green);

					SpawnedMesh->GetStaticMeshComponent()->SetMaterial(0, DynamicMatInstance);
				}
				
			}
		}
	}
}

void AZTPlayerController::ProcessMouseClick()
{
	float MouseX, MouseY;
	if(GetMousePosition(MouseX, MouseY))
	{
		FVector WorldLocation, WorldDirection;
		if(DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			FHitResult HitResult;
			FVector TraceEnd = WorldLocation + WorldDirection * 10000.0f;

			FCollisionQueryParams QueryParams;
			QueryParams.bTraceComplex = true;
			QueryParams.AddIgnoredActor(GetPawn());

			if(GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams))
			{
				if(HitResult.GetActor())
				{
					LastHitLocation = HitResult.Location;
					if(OverlayWidget && !OverlayWidget->IsInViewport())
					{
						OverlayWidget->AddToViewport();
						OverlayWidget->InitializeWidget(MeshDataAsset);
					}
					
					OnFloorDetected();
				}
			}
			
		}
	}
}
