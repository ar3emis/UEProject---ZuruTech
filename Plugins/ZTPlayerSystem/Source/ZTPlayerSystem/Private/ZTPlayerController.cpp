// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTPlayerController.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SubSystems/ZTGameInstanceSubsystem.h"

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

		UInputAction* SaveAction = NewObject<UInputAction>();
		SaveAction->ValueType = EInputActionValueType::Boolean;
		InputMappingContext->MapKey(SaveAction, EKeys::Tab);
		EnhancedInputComponent->BindAction(SaveAction, ETriggerEvent::Triggered, this, &AZTPlayerController::SaveSlot);

		
		UInputAction* LoadAction = NewObject<UInputAction>();
		LoadAction->ValueType = EInputActionValueType::Boolean;
		InputMappingContext->MapKey(LoadAction, EKeys::L);
		EnhancedInputComponent->BindAction(LoadAction, ETriggerEvent::Triggered, this, &AZTPlayerController::LoadSlot);


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
			SpawnedMesh->Tags.Add("PlayerSpawned");
			if(BaseMaterial)
			{
				if(UMaterialInstanceDynamic* DynamicMatInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this))
				{
					DynamicMatInstance->SetScalarParameterValue("Tiling", 5);
					DynamicMatInstance->SetVectorParameterValue("BaseColor", FLinearColor::Green);

					SpawnedMesh->GetStaticMeshComponent()->SetMaterial(0, DynamicMatInstance);
				}
				
			}

			FSaveSlotData SaveSlotData;
			SaveSlotData.ActorName = SpawnedMesh->GetName();
			SaveSlotData.ActorMesh = MeshData.Mesh;
			SaveSlotData.ActorTransform = SpawnedMesh->GetActorTransform();

			SaveSlotMeshInfo.Add(SaveSlotData);
		
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

void AZTPlayerController::SaveSlot()
{
	if(UZTGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UZTGameInstanceSubsystem>())
	{
		FString CurrentSaveSlotName = Subsystem->GetCurrentSaveSlotName();
		if(CurrentSaveSlotName.IsEmpty())
		{
			Subsystem->CreateSaveGameSlot("ZTSaveSlot");	
		}

		for(int iIndex = 0; iIndex < SaveSlotMeshInfo.Num(); iIndex++)
		{
			
			FSaveSlotData SaveSlotData = SaveSlotMeshInfo[iIndex];
			FZTSaveSlot& SaveSlot = Subsystem->GetCurrentSaveGame()->SaveSlots.FindOrAdd(CurrentSaveSlotName);
			SaveSlot.SavedActors.Add(SaveSlotData);
		}
		
		Subsystem->SaveGame();
	}
}

void AZTPlayerController::LoadSlot()
{
	if(UZTGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UZTGameInstanceSubsystem>())
	{
		
		FString SlotName = "ZTSaveSlot";

		Subsystem->LoadGame(SlotName);
		
		FString CurrentSaveSlotName = Subsystem->GetCurrentSaveSlotName();
		if(!CurrentSaveSlotName.IsEmpty())
		{
		
			UZTPlayerSaveGame* CurrentSaveGame = Subsystem->GetCurrentSaveGame();
			if(FZTSaveSlot* SaveSlot = CurrentSaveGame->SaveSlots.Find(SlotName))
			{
				TArray<AActor*> ActorstoDestroy;
				for(TActorIterator<AActor> It(GetWorld()); It; ++It)
				{
					AActor* Actor = *It;
					if(Actor->ActorHasTag("PlayerSpawned"))
					{
						ActorstoDestroy.Add(Actor);
					}
				}

				for(auto& Actor : ActorstoDestroy)
				{
					Actor->Destroy();
				}

				
				for(auto& SaveData :SaveSlot->SavedActors)
				{
					FActorSpawnParameters SpawnParameters;
					SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					if(AStaticMeshActor* SpawnedMesh = GetWorld()->SpawnActor<AStaticMeshActor>(SaveData.ActorTransform.GetLocation(), SaveData.ActorTransform.GetRotation().Rotator(), SpawnParameters))
					{
						SpawnedMesh->SetActorRelativeScale3D(SaveData.ActorTransform.GetScale3D());
						
						SpawnedMesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
						SpawnedMesh->GetStaticMeshComponent()->SetStaticMesh(SaveData.ActorMesh);
						SpawnedMesh->SetActorLabel(SaveData.ActorName);
						
						SpawnedMesh->Tags.Add("PlayerSpawned");
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
			
		}
	}
}
